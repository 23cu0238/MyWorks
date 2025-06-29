// ------------------------------------------------
// PlayerCharacter.cpp
// プレーヤーキャラクターを制御するクラス
// 作成者：李朗曦、浅川皓亮
// 作成日時：2024-11-18
// ------------------------------------------------

#include "PlayerCharacter.h"
#include "GravityController.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerAttack.h"
#include "PlayerAttackRange.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "MyGameInstance.h"

//コンストラクタ
APlayerCharacter::APlayerCharacter(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UGravityCharacterMovementComp>(TEXT("UGravityCharacterMovementComp")))
{
	// 毎フレームTick()を呼び出すように設定します
	PrimaryActorTick.bCanEverTick = true;

	// カメラブームを作る（衝突があった場合、プレーヤーの方に引き寄せられる）
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;		// カメラはこの距離でキャラクターの背後を追う
	CameraBoom->bUsePawnControlRotation = true; // コントローラーに基づいてアームを回転させる

	// 肩越しカメラブームを作る（衝突があった場合、プレーヤーの方に引き寄せられる）
	FirstPersonCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("FirstPersonCameraBoom"));
	FirstPersonCameraBoom->SetupAttachment(ACharacter::GetMesh());
	FirstPersonCameraBoom->bUsePawnControlRotation = true; // コントローラーに基づいてアームを回転させる

	//  肩越しカメラを作る
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // カメラをブームの先端に取り付け、コントローラーの向きに合わせてブームを調整する
	FollowCamera->bUsePawnControlRotation = true;								// カメラがアームに対して回転しない
	ActiveCamera = FollowCamera;

	// カメラを作る
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(FirstPersonCameraBoom); // カメラをブームの先端に取り付け、コントローラーの向きに合わせてブームを調整する
	FirstPersonCamera->bUsePawnControlRotation = true;		   // カメラがアームに対して回転しない
	FirstPersonCamera->SetActive(false);

	// コリジョンカプセルのサイズを設定
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// アイテムとインタラクション範囲を作成
	mInteractComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRange"));
	mInteractComponent->SetupAttachment(GetCapsuleComponent());

	// アタックの範囲を作成
	mAttackComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackComp"));
	mAttackComponent->SetupAttachment(GetCapsuleComponent());

	// コントローラーが回転しても回転させない
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// キャラクターの動きを設定する
	GetCharacterMovement()->bOrientRotationToMovement = true;			 // 入力された方向にキャラクターが動く
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 500.0f); // この回転数で

	// 必要な値を定義
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// 重力ゲージの最大値を設定
	mGravityGage = mMaxGage = 100;

	// 音声データを取得
	static ConstructorHelpers::FObjectFinder<USoundCue> HitSoundCueTemp(
		TEXT("/Script/Engine.SoundCue'/Game/SE/HitCue.HitCue'"));

	if (HitSoundCueTemp.Succeeded())
	{
		HitSoundCue = HitSoundCueTemp.Object;
	}

	// 音声コンポーネントを作成
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComponent->bAutoActivate = false;
	// ルートにアタッチ
	AudioComponent->SetupAttachment(RootComponent);

	// タイムラインを作成
	FirstPersonTimeline = new FTimeline{};
	ThirdPersonTimeline = new FTimeline{};

	// カーブを取得
	const ConstructorHelpers::FObjectFinder<UCurveFloat> _Find(TEXT("/Script/Engine.CurveFloat'/Game/System/TimelineCurve.TimelineCurve'"));

	if (_Find.Succeeded())
	{
		TimelineCurve = _Find.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load curve."));
	}

	if (TimelineCurve)
	{
		if (FirstPersonTimeline != nullptr)
		{
			// タイムライン更新時に呼び出されるメソッドの定義
			FOnTimelineFloat TimelineStepFunc;
			TimelineStepFunc.BindUFunction(this, TEXT("FirstPersonTimelineStep"));
			FirstPersonTimeline->AddInterpFloat(TimelineCurve, TimelineStepFunc);

			// タイムライン終了時に呼び出されるメソッドの定義
			FOnTimelineEvent TimelineFinishedFunc;
			TimelineFinishedFunc.BindUFunction(this, TEXT("FirstPersonTimelineFinished"));
			FirstPersonTimeline->SetTimelineFinishedFunc(TimelineFinishedFunc);
		}

		if (ThirdPersonTimeline != nullptr)
		{
			// タイムライン更新時に呼び出されるメソッドの定義
			FOnTimelineFloat TimelineStepFunc;
			TimelineStepFunc.BindUFunction(this, TEXT("ThirdPersonTimelineStep"));
			ThirdPersonTimeline->AddInterpFloat(TimelineCurve, TimelineStepFunc);

			// タイムライン終了時に呼び出されるメソッドの定義
			FOnTimelineEvent TimelineFinishedFunc;
			TimelineFinishedFunc.BindUFunction(this, TEXT("ThirdPersonTimelineFinished"));
			ThirdPersonTimeline->SetTimelineFinishedFunc(TimelineFinishedFunc);
		}
	}
}

// デストラクタ
APlayerCharacter::~APlayerCharacter()
{
	if (FirstPersonTimeline)
	{
		delete FirstPersonTimeline;
	}

	if (ThirdPersonTimeline)
	{
		delete ThirdPersonTimeline;
	}
}

// スタート関数
void APlayerCharacter::BeginPlay()
{
	// 親クラスのBeginPlay
	Super::BeginPlay();

	// HPの初期値を設定
	mHitPoint = 3;

	// 初期重力スケールを記録
	mDefaultGravityScale = GetCharacterMovement()->GravityScale;
	mDefaultGravityDirection = GetGravityDirection();

	// コリションとOnHit関数のバインド
	UCapsuleComponent *CollisionComp = GetCapsuleComponent();
	CollisionComp->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnHit);

	// アタック範囲とアタック関数をバインド
	mAttackComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnAttackHit);

	// 操作マッピングを追加する
	if (APlayerController *PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// 各カメラの初期位置を記録
	FirstPersonCameraOriginalPosition = FirstPersonCamera->GetRelativeTransform();
	ThirdPersonCameraOriginalPosition = FollowCamera->GetRelativeTransform();

	// 音声を設定
	if (HitSoundCue->IsValidLowLevelFast())
	{
		AudioComponent->SetSound(HitSoundCue);
	}

	// レスポンス位置を設定
	UMyGameInstance *instance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (instance)
		SetActorLocation(instance->GetInstance()->GetRespawnPosition());
}

// 毎フレーム更新
void APlayerCharacter::Update()
{
	// 死亡判定
	if (Dead())
	{
		mPlayerStatus = EPlayerStatus::Died;
	}

	// 重力変更していない時と地面に移動している時カメラのローテーションラグを無くす
	if (bNotChangingGravity && GetCharacterMovement()->IsMovingOnGround())
	{
		CameraBoom->bEnableCameraRotationLag = false;
	}

	// 地面移動している時
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		// 加速度、重力スケールをリセット
		mAcceleration = 1;
		GetCharacterMovement()->GravityScale = mDefaultGravityScale;

		// FOV（視野角）をリセット
		if (FirstPersonCamera->FieldOfView != 90.0f)
		{
			FirstPersonCamera->FieldOfView = FMath::FInterpTo(FirstPersonCamera->FieldOfView, 90.0f, GetWorld()->GetTime().GetDeltaRealTimeSeconds(), 5.0f);
		}

		// プレーヤーのカメラのピッチをロック
		Cast<APlayerController>(Controller)->PlayerCameraManager->ViewPitchMax = 50.0f;
	}
	else
	{
		// 重力加速度を増やす
		GetCharacterMovement()->GravityScale += 0.1;

		// プレーヤーのカメラのピッチをアンロック（ほぼ９０度にする）
		Cast<APlayerController>(Controller)->PlayerCameraManager->ViewPitchMax = 89.900002f;
	}

	// 重力変更中であれば
	if (mPlayerStatus == EPlayerStatus::ChangingDirection)
	{
		// ジャンプした後、落下開始したら
		FVector LocalVelocity = UKismetMathLibrary::LessLess_VectorRotator(GetVelocity(), GetActorRotation());
		if (LocalVelocity.Z < -0.001)
		{
			// 重力変更モードに入ってる時
			if (bNotChangingGravity)
			{
				// カメラの変更タイムラインを開始
				FirstPersonTimeline->PlayFromStart();
				// 重力変更モードに入ったフラグ
				bNotChangingGravity = false;
				// カメラのラグを無効にする
				CameraBoom->bEnableCameraRotationLag = false;
			}

			// カメラの変更が終わった後
			if (!bIsCameraTransitioning)
			{
				// プレイヤー自身のを無重力状態にする
				GetCharacterMovement()->GravityScale = 0.0;
				GetCharacterMovement()->Velocity = FVector::ZeroVector;

				// 時間の流れを遅くする
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), FMath::FInterpTo(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), mSlowMotionTimeScale, GetWorld()->GetTime().GetDeltaRealTimeSeconds(), 10.0f));
			}
		}

		// 重力変更ステータスに入ったけど、変更モードが発動されていない時
		if (GetCharacterMovement()->IsMovingOnGround() && !bIsCameraTransitioning && bNotChangingGravity)
		{
			mPlayerStatus = EPlayerStatus::Normal;
		}
	}

	// 重力変更中でなければ
	else
	{
		// 時間の流れ速度を元に戻す
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), FMath::FInterpTo(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), 1.0f, GetWorld()->GetTime().GetDeltaRealTimeSeconds(), 10.0f));
	}

	// 一般の状態
	if (mPlayerStatus == EPlayerStatus::Normal)
	{
		// クイック重力変更可能かどうかのチェック
		bIsAbleToChangeGravityQuick = CheckIsAbleToChangeGravityQuick();

		// もしFirstPersonCameraが有効し、重力変更した場合
		if (FirstPersonCamera->IsActive() && !bNotChangingGravity)
		{
			// カメラの切り替え
			ThirdPersonTimeline->PlayFromStart();
			// 重力変更していないフラグを立つ
			bNotChangingGravity = true;
		}

		// カスタム重力がないし、重力ゲージが１００％以下の場合、１００％まで増やす
		if (!GetCharacterMovement()->HasCustomGravity() && mGravityGage < 100.0f)
		{
			mGravityGage += 20.0f * GetWorld()->GetDeltaSeconds();
			if (mGravityGage > 100.0f)
			{
				mGravityGage = 100.0f;
			}
		}

		// カスタム重力がある場合、重力ゲージをどんどん減らす
		else if (GetCharacterMovement()->HasCustomGravity())
		{
			mGravityGage -= 10.0f * GetWorld()->GetDeltaSeconds();
		}

		// 重力ゲージがなくなった場合は重力方向をリセット
		if (mGravityGage <= 0)
		{
			GetCharacterMovement()->SetGravityDirection(FVector::DownVector);
		}
	}

	// 現実世界のデルタタイムを取得
	float RealTimeDeltaSeconds = GetWorld()->GetTime().GetDeltaRealTimeSeconds();

	// カメラのタイムラインが再生されている場合はそのタイムラインをチック
	if (FirstPersonTimeline->IsPlaying())
	{
		FirstPersonTimeline->TickTimeline(RealTimeDeltaSeconds);
	}
	if (ThirdPersonTimeline->IsPlaying())
	{
		ThirdPersonTimeline->TickTimeline(RealTimeDeltaSeconds);
	}

	// 重力変更後、着地までの時間を計測
	if (bIsGravityChanged)
	{
		mChangedGravityTimer += GetWorld()->GetDeltaSeconds();
	}

	// 変更した瞬間の接触判定を防止
	if (mChangedGravityTimer >= 0.02f)
	{
		bIsWaitingToHitGround = true;
	}

	// 現在のパワーレベルを判定
	for (int i = 0; i < mPowerLevelTimeThreshold.Num(); i++)
	{
		if (mChangedGravityTimer >= mPowerLevelTimeThreshold[i])
		{
			mPowerLevel = i;
		}

		else
		{
			break;
		}
	}

	// 落下攻撃中の処理
	if (mPlayerStatus == EPlayerStatus::Falling)
	{
		// 変更した瞬間の接触判定を防止フラグを立った後
		if (bIsWaitingToHitGround)
		{
			// FOVをスムーズに広くする
			FirstPersonCamera->FieldOfView = FMath::FInterpTo(FirstPersonCamera->FieldOfView, 135.0f, GetWorld()->GetTime().GetDeltaRealTimeSeconds(), 5.0f);
		}
		// アタック範囲を有効
		mAttackComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	}
	else
	{
		// アタック範囲を無効
		mAttackComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 一時停止していない時
	if (mPauseMenuWidgetInstance && !mPauseMenuWidgetInstance->IsInViewport())
	{
		Cast<APlayerController>(GetController())->bShowMouseCursor = false;
	}
}

// 元の重力方向に戻る関数
void APlayerCharacter::BackToNormalGravity()
{
	if (GetGravityDirection() != FVector::DownVector)
	{
		// カメラを通常時に戻す
		ThirdPersonTimeline->PlayFromStart();
		FirstPersonCamera->FieldOfView = FMath::FInterpTo(FirstPersonCamera->FieldOfView, 90.0f, GetWorld()->GetTime().GetDeltaRealTimeSeconds(), 5.0f);

		// 重力方向をリセット
		GetCharacterMovement()->SetGravityDirection(FVector::DownVector);

		// フラグとステータスとタイマーをリセット
		bIsWaitingToHitGround = false;
		bIsGravityChanged = false;
		mPlayerStatus = EPlayerStatus::Normal;
		mChangedGravityTimer = 0.0f;
	}
}

// 移動処理
void APlayerCharacter::Move(const FInputActionValue &Value)
{
	// 死亡時なんもしない
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return;
	}

	// 移動処理ベクトルを取得
	FVector2D MovementVector = Value.Get<FVector2D>();

	// コントローラが存在する時
	if (AGravityController *GravityController = Cast<AGravityController>(Controller))
	{
		// カメラのローテーションを取得
		const FRotator Rotation = Controller->GetControlRotation();

		// 重力方向に応じた回転の補正
		const FRotator GravityRelativeRotation = GravityController->GetGravityRelativeRotation(Rotation, GetGravityDirection());
		const FRotator GravityWorldRotation = GravityController->GetGravityWorldRotation(GravityRelativeRotation, GetGravityDirection());

		// 現在の前ベクトルと右ベクトルを取得
		const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(GravityWorldRotation);
		const FVector RightDirection = UKismetMathLibrary::GetRightVector(GravityWorldRotation);

		// 移動させる
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

// 重力変更をキャンセル
void APlayerCharacter::CancelGravity()
{
	// 死亡時なんもしない
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return;
	}

	// カメラが変更していないとき
	if (!bIsCameraTransitioning)
	{
		// 変更中
		if (mPlayerStatus == EPlayerStatus::ChangingDirection && ActiveCamera == FirstPersonCamera)
		{
			// ステータスをリセット
			mPlayerStatus = EPlayerStatus::Normal;
		}

		// 落下攻撃中
		else if (mPlayerStatus == EPlayerStatus::Falling)
		{
			// 重力をリセット
			BackToNormalGravity();
		}
	}
}

// 重力を決定じの処理
void APlayerCharacter::ApplyGravity()
{
	// 死亡時なんもしない
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return;
	}

	// 重力変更中ではない時は何もしない
	if (mPlayerStatus != EPlayerStatus::ChangingDirection)
	{
		return;
	}

	// 重力変更中とカメラ変更していない時
	if (!bNotChangingGravity && !bIsCameraTransitioning)
	{
		// 重力加速度をリセット、方向をカメラの正面にする
		GetCharacterMovement()->GravityScale = 1.0;
		GetCharacterMovement()->SetGravityDirection(FollowCamera->GetForwardVector());

		// ステータスを落下攻撃中にする
		mPlayerStatus = EPlayerStatus::Falling;

		// フラグを立つ
		bIsGravityChanged = true;
		bNotChangingGravity = true;

		// カメラの回転ラグを有効化
		CameraBoom->bEnableCameraRotationLag = true;

		// 重力目標地点の情報を消す
		bNotHit();
	}
}

// クイック重力変更可能かどうかをチェックする関数
bool APlayerCharacter::CheckIsAbleToChangeGravityQuick()
{
	// 死亡時なんもしない
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return false;
	}

	// ステータスが通常時だけ
	if (mPlayerStatus == EPlayerStatus::Normal)
	{
		// レイキャストを使って、カメラの中央にあるオブジェクトが歩ける地面かどうかをチェック
		// 歩ける場合は重力変更目標地点を記録し、trueを返す
		// 歩けない場合重力変更目標地点を消す、falseを返す
		FHitResult Hit;
		FVector StartLoc = GetActiveCamera()->GetComponentLocation();
		FVector EndLoc = StartLoc + GetActiveCamera()->GetForwardVector() * 2500.0f;
		FCollisionQueryParams COQP;
		COQP.AddIgnoredActor(this);
		FCollisionResponseParams CollRes;

		if (GetWorld()->LineTraceSingleByChannel(Hit, StartLoc, EndLoc, ECollisionChannel::ECC_Visibility, COQP, CollRes))
		{
			if (Hit.GetActor()->ActorHasTag("Walkable"))
			{
				mTargetGravityDirection = UKismetMathLibrary::GetDirectionUnitVector(Hit.ImpactNormal * 100.0f + Hit.ImpactPoint, Hit.ImpactPoint);

				GravitySpot.bIsHit = true;
				GravitySpot.SpotLocation = Hit.Location;
				GravitySpot.SpotNormal = -mTargetGravityDirection;
				return true;
			}
			else
			{
				bNotHit();
				return false;
			}
		}
		else
		{
			bNotHit();
			return false;
		}
	}
	else
	{
		bNotHit();
		return false;
	}
}

// クイック重力変更関数
void APlayerCharacter::ChangeGravityQuick()
{
	// 死亡時なんもしない
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return;
	}

	// ステータスが通常時だけ
	if (mPlayerStatus == EPlayerStatus::Normal)
	{
		// レイキャストを使って、カメラの中央にあるオブジェクトが歩ける地面であれば、
		// 重力方向をその面の垂直方向にする
		FHitResult Hit;
		FVector StartLoc = GetActiveCamera()->GetComponentLocation();
		FVector EndLoc = StartLoc + GetActiveCamera()->GetForwardVector() * 2500.0f;
		FCollisionQueryParams COQP;
		COQP.AddIgnoredActor(this);
		FCollisionResponseParams CollRes;

		if (GetWorld()->LineTraceSingleByChannel(Hit, StartLoc, EndLoc, ECollisionChannel::ECC_Visibility, COQP, CollRes))
		{
			if (Hit.GetActor()->ActorHasTag("Walkable"))
			{

				GravitySpot.bIsHit = true;
				GravitySpot.SpotLocation = Hit.Location;
				GravitySpot.SpotNormal = -mTargetGravityDirection;

				GetCharacterMovement()->SetGravityDirection(UKismetMathLibrary::GetDirectionUnitVector(Hit.ImpactNormal * 100.0f + Hit.ImpactPoint, Hit.ImpactPoint));
			}
		}
	}
}

// 重力変更関数
void APlayerCharacter::ChangeGravity()
{
	// 死亡時なんもしない
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return;
	}

	// カメラが変更中ではない時だけ
	if (!bIsCameraTransitioning)
	{
		// ステータスが通常時だけ
		if (mPlayerStatus == EPlayerStatus::Normal)
		{
			// 一回ジャンプして、ステータスを重力変更中にする
			ACharacter::Jump();
			mPlayerStatus = EPlayerStatus::ChangingDirection;
		}
	}
}

// ヒット処理
void APlayerCharacter::OnHit(UPrimitiveComponent *HitCom, AActor *OtherActor, UPrimitiveComponent *OtherComp,
							 FVector NormalImpulse, const FHitResult &Hit)
{
	// 死亡時なんもしない
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return;
	}

	// ステータスが通常時であれば何もしない
	if (mPlayerStatus == EPlayerStatus::Normal)
	{
		return;
	}

	// 変更した瞬間の接触判定を防止フラグが立てる時と接触しているオブジェクトが歩ける地面であれば
	if (bIsWaitingToHitGround && OtherActor->ActorHasTag("Walkable"))
	{
		// 速度をゼロにする
		GetCharacterMovement()->Velocity = FVector::ZeroVector;

		// カメラを変更
		ThirdPersonTimeline->PlayFromStart();

		// 重力方向を接触している地面の垂直方向にする
		GetCharacterMovement()->SetGravityDirection(UKismetMathLibrary::GetDirectionUnitVector(Hit.ImpactNormal * 100.0f + Hit.ImpactPoint, Hit.ImpactPoint));

		// ステータス、フラグをリセット
		mPlayerStatus = EPlayerStatus::Normal;
		bIsWaitingToHitGround = false;
		bIsGravityChanged = false;

		// 爆風を生成
		SpawnExplosionDelegate.Broadcast(mPowerLevel);

		// 重力変更タイマーをリセット
		mChangedGravityTimer = 0.0f;
	}
}

// アタック処理
void APlayerCharacter::OnAttackHit(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
								   int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// 死亡時なんもしない
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return;
	}

	// 接触したのはエネミーの場合
	if (OtherActor->ActorHasTag("Enemy"))
	{
		// 音声を再生
		AudioComponent->Play();

		// 相手のHPを減らす
		TObjectPtr<UConventionalAttack> attackObj = NewObject<UConventionalAttack>();
		attackObj->Attack((float)mPowerLevel, OtherActor);
	}
}

// 肩越しカメラに変更するタイムラインのチック
void APlayerCharacter::FirstPersonTimelineStep(float _Value)
{
	if (FirstPersonCamera && FollowCamera)
	{
		// 変更前のカメラをスムーズに新カメラの位置に移動
		FTransform newTransform = UKismetMathLibrary::TLerp(FollowCamera->GetComponentTransform(), FirstPersonCamera->GetComponentTransform(), _Value);
		FollowCamera->SetWorldTransform(newTransform);

		// プレーヤーキャラクターをカメラの正面にスムーズに回転させる
		SetActorRotation(FQuat::Slerp(GetActorRotation().Quaternion(), GetControlRotation().Quaternion(), 0.3f));
	}
}

// 一般カメラに変更するタイムラインのチック
void APlayerCharacter::ThirdPersonTimelineStep(float _Value)
{
	if (FirstPersonCamera && FollowCamera)
	{
		// 変更前のカメラをスムーズに新カメラの位置に移動
		FTransform newTransform = UKismetMathLibrary::TLerp(FirstPersonCamera->GetComponentTransform(), FollowCamera->GetComponentTransform(), _Value);
		FirstPersonCamera->SetWorldTransform(newTransform);
	}
}

// 肩越しカメラに変更するタイムライン再生完了するときの処理
void APlayerCharacter::FirstPersonTimelineFinished()
{
	if (FirstPersonCamera && FollowCamera)
	{
		//目標カメラを有効化し、不要なカメラを無効化
		FirstPersonCamera->SetActive(true);
		FollowCamera->SetActive(false);
		ActiveCamera = FirstPersonCamera;
		
		//不要なカメラの位置を元の位置に戻す
		FollowCamera->SetRelativeTransform(ThirdPersonCameraOriginalPosition);

		//肩越しカメラの回転を更新
		UpdateFPSCameraRotation();

		//プレイヤーの回転をカメラと揃える
		bUseControllerRotationPitch = true;
		bUseControllerRotationRoll = true;
		bUseControllerRotationYaw = true;

		//カメラ移動完成フラグを立つ
		bIsCameraTransitioning = false;
	}
}

// 通常カメラに変更するタイムライン再生完了するときの処理
void APlayerCharacter::ThirdPersonTimelineFinished()
{
	if (FirstPersonCamera && FollowCamera)
	{
		//目標カメラを有効化し、不要なカメラを無効化
		FollowCamera->SetActive(true);
		FirstPersonCamera->SetActive(false);
		ActiveCamera = FollowCamera;

		//不要なカメラの位置を元の位置に戻す
		FirstPersonCamera->SetRelativeTransform(FirstPersonCameraOriginalPosition);
		
		//プレイヤーの回転をカメラと揃えないようにする
		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		bUseControllerRotationYaw = false;

		//カメラ移動完成フラグを立つ
		bIsCameraTransitioning = false;
	}
}

// 肩越しカメラの回転を更新
void APlayerCharacter::UpdateFPSCameraRotation()
{
	//重力方向を宣言
	FVector GravityDirection = FVector::DownVector;

	if (UCharacterMovementComponent *MoveComp = GetCharacterMovement())
	{
		//キャラクタームーブメントコンポーネントが存在すればそれから現在の重力方向を取得する
		GravityDirection = MoveComp->GetGravityDirection();
	}

	// ワールド空間で現在のコントロール回転を取得する
	FRotator ViewRotation = GetActorForwardVector().Rotation();

	// ビューの回転をワールド空間から重力相対空間に変換する
	// Now we can work with the rotation as if no custom gravity was affecting it.
	ViewRotation = AGravityController::GetGravityRelativeRotation(ViewRotation, GravityDirection);

	// ViewRotationに適用されるデルタを計算する
	FRotator DeltaRot(Cast<APlayerController>(Controller)->RotationInput);

	if (TObjectPtr<APlayerCameraManager> CameraManager = Cast<APlayerController>(Controller)->PlayerCameraManager)
	{
		CameraManager->ProcessViewRotation(GetWorld()->GetTime().GetDeltaRealTimeSeconds(), ViewRotation, DeltaRot);

		// カメラのロールをゼロにし、重力に対して常に水平にする
		ViewRotation.Roll = 0;

		// 回転をワールド空間に戻し、現在のコントロール回転として設定する
		Controller->SetControlRotation(AGravityController::GetGravityWorldRotation(ViewRotation, GravityDirection));

		ViewRotation = GetControlRotation();
	}
}

// 入力関数をバインド
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	// アクションバインディングの設定
	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		// ジャンプ
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// 移動
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// カメラ制御
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		// 重力変更
		EnhancedInputComponent->BindAction(ChangeGravityAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ChangeGravity);

		// 重力変更決定
		EnhancedInputComponent->BindAction(ApplyGravityAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ApplyGravity);

		// 重力変更キャンセル
		EnhancedInputComponent->BindAction(GravityCancelAction, ETriggerEvent::Triggered, this, &APlayerCharacter::CancelGravity);

		// クイック重力変更
		EnhancedInputComponent->BindAction(QuickGravityChangeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ChangeGravityQuick);

		// チャージ
		EnhancedInputComponent->BindAction(ChargeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Charge);

		// 一時停止
		EnhancedInputComponent->BindAction(CommandAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Pause);
	}
}

// HP減らす処理
void APlayerCharacter::FluctuationHP(float decrease)
{
	//落下攻撃中は無敵
	if (mPlayerStatus == EPlayerStatus::Falling)
	{
		return;
	}

	//親クラスのHP減らす処理
	Super::FluctuationHP(decrease);
}

// カメラ制御
void APlayerCharacter::Look(const FInputActionValue &Value)
{
	//カメラ切り替え中であれば処理しない
	if (bIsCameraTransitioning)
		return;

	//軸の値を取得する
	FVector LookAxisVector = Value.Get<FVector>();

	if (Controller != nullptr)
	{
		// コントローラにヨーとピッチの入力を追加する
		AddControllerYawInput(LookAxisVector.X * mCameraSensitivity);
		AddControllerPitchInput(LookAxisVector.Y * mCameraSensitivity);
	}
}

// 一時停止
void APlayerCharacter::Pause()
{
	//一時停止メニューが存在すれば
	if (mPauseMenuWidget)
	{
		//一時停止メニューのインスタンスが存在しなければ作成する
		if (!mPauseMenuWidgetInstance)
		{
			mPauseMenuWidgetInstance = CreateWidget(Cast<APlayerController>(Controller), mPauseMenuWidget);
		}

		//既に表示中であれば非表示にし、非表示中であれば表示にする
		if (!mPauseMenuWidgetInstance->IsInViewport())
		{
			mPauseMenuWidgetInstance->AddToViewport();
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
		else
		{
			mPauseMenuWidgetInstance->RemoveFromViewport();
			UGameplayStatics::SetGamePaused(GetWorld(), false);

			Cast<APlayerController>(GetController())->bShowMouseCursor = false;
		}
	}
}

// チャージ（廃案）
void APlayerCharacter::Charge()
{
	return;

	if (mPlayerStatus == EPlayerStatus::ChangingDirection)
	{
		mChangedGravityTimer += GetWorld()->GetTime().GetDeltaRealTimeSeconds() * 2;
		mGravityGage -= GetWorld()->GetTime().GetDeltaRealTimeSeconds() * 5.0f;
		if (mGravityGage <= 0)
		{
			mGravityGage = 0;
			ChangeGravity();
		}
	}
}