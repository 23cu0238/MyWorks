// Fill out your copyright notice in the Description page of Project Settings.

//--------------------------------------------------------
// BossEnemy.cpp
// 敵ボスの実装
// 作成者：23cu0229 日野村 悠人
//--------------------------------------------------------

#include "BossEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PlayerAttack.h"
#include "PlayerCharacter.h"
#include "ShootingAttack.h"
#include "CollisionQueryParams.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Components/LineBatchComponent.h"
#include "ShootingAttack.h"


// コンストラクタ
ABossEnemy::ABossEnemy()
{
	// 攻撃範囲設定
	mAttackComp = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackComp"));

	FVector NewExtent(10.f, 10.f, 10.f);
	mAttackComp->SetBoxExtent(NewExtent);
	FVector NewRelativeLocation(10.f, 10.f, 10.f); // 親からの相対位置
	mAttackComp->SetRelativeLocation(NewRelativeLocation);

	FName fnAttackSocket = TEXT("Attack");
	mAttackComp->SetupAttachment(GetMesh(), fnAttackSocket);

	// 近接攻撃範囲設定
	mCloseAttackComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CloseAttackComp"));

	mCloseAttackComp->SetBoxExtent(NewExtent);
	// 親からの相対位置
	mCloseAttackComp->SetRelativeLocation(NewRelativeLocation);

	FName fnCloseAttackSocket = TEXT("CloseAttack");
	mCloseAttackComp->SetupAttachment(GetMesh(), fnCloseAttackSocket);
}

// デストラクタ
ABossEnemy::~ABossEnemy()
{
}

// 実行時に処理
void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 初期位置
	mInitPos = GetActorLocation();

	// 攻撃範囲の設定
	mAttackComp->SetActive(true);
	mCloseAttackComp->SetActive(true);

	// Overlapイベントの登録
	mAttackComp->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemy::OnOverlapAttack);
	mAttackComp->OnComponentEndOverlap.AddDynamic(this, &ABossEnemy::OnOverlapAttackEnd);
	mCloseAttackComp->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemy::OnOverlapAttack);
	mCloseAttackComp->OnComponentEndOverlap.AddDynamic(this, &ABossEnemy::OnOverlapAttackEnd);

	mShootiongAttack = NewObject<UShootingAttack>();
	mShootiongAttack->Init(this, mShootingRadius, GetWorld(), mShootingName);

	// 衝突イベントを登録
	// mCloseAttackComp->OnComponentHit.AddDynamic(this, &ABossEnemy::OnHit);
}

// 毎フレーム更新処理
void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 移動更新処理
	UpdateMove(DeltaTime);

	// すでに攻撃中 or クールタイム中なら何もしない
	// if (mIsActiveAttack/* || GetWorldTimerManager().IsTimerActive(mAttackTimerHandle)*/)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("InAction"));
	//	return;
	//}
	// プレイヤーを発見した場合、攻撃開始
	bool bDetected = SearchPlayer();
	UE_LOG(LogTemp, Log, TEXT("Boss:SearchPlayer result: %s"), bDetected ? TEXT("True") : TEXT("False"));

	if (!GetWorldTimerManager().IsTimerActive(mAttackTimerHandle) && SearchPlayer())
	{
		UE_LOG(LogTemp, Log, TEXT("Boss:Player detected, starting BossAttack."));
		GetWorldTimerManager().SetTimer(mAttackTimerHandle, this, &ABossEnemy::BossAttack, 0.1f, false);

	}

	if (bIsTargetOverlappingAttack)
	{
		if (mOverlapOtherActor && mOverlapOtherComp)
			AttackFunction(mOverlapOtherActor, mOverlapOtherComp);
	}

	// if (GEngine && GetCharacterMovement())
	//{
	//	FString VelocityStr = FString::Printf(TEXT("Velocity: %s"), *GetCharacterMovement()->Velocity.ToString());
	//	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, VelocityStr);

	//	FString MovementModeStr = FString::Printf(TEXT("Movement Mode: %d"), (int)GetCharacterMovement()->MovementMode);
	//	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, MovementModeStr);

	//	FString IsMovingOnGround = GetCharacterMovement()->IsMovingOnGround() ? TEXT("True") : TEXT("False");
	//	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, TEXT("Is Moving On Ground: ") + IsMovingOnGround);
	//}
}

// 移動更新処理
void ABossEnemy::UpdateMove(float DeltaTime)
{
	// 攻撃中は処理しない
	if (mbCanAttack || mIsActiveAttack)
		return;

	if (SearchPlayer())
	{
		if (mOverlapOtherActor)
		{
			mPlayerPos = mOverlapOtherActor->GetActorLocation();
			FindAndMoveToLocation(mPlayerPos);
		}
	}

	switch (mCStatus)
	{
	case ChaseStatus::None:
		break;

		// ステータスがIdleの場合
	case ChaseStatus::Idle:
		// SearchPlayerに自分の座標と前ベクトルを渡す
		if (SearchPlayer())
		{
			// プレイヤーを見つけたらステータスをChaseに変更
			// UE_LOG(LogTemp, Log, TEXT("Player detected, switching to Chase state."));
			mCStatus = ChaseStatus::Chase;
		}
		break;

		// ステータスがChaseの場合
	case ChaseStatus::Chase:
		// SearchPlayerに自分の座標と前ベクトルを渡す
		if (SearchPlayer())
		{
			// プレイヤーに向かって移動
			// UE_LOG(LogTemp, Log, TEXT("Chasing player at position: %s"), *mPlayerPos.ToString());
			FindAndMoveToLocation(mPlayerPos);

			bool bDetected = SearchPlayer();
			UE_LOG(LogTemp, Log, TEXT("BossChase:SearchPlayer result: %s"), bDetected ? TEXT("True") : TEXT("False"));
		}
		// 見失った場合
		else
		{
			bool bDetected = SearchPlayer();
			// UE_LOG(LogTemp, Log, TEXT("SearchPlayer result: %s"), bDetected ? TEXT("True") : TEXT("False"));

			// プレイヤーが最後にいた位置を記録し、ステータスをTatgetLostに変更
			// UE_LOG(LogTemp, Warning, TEXT("Player lost, switching to TargetLost state."));
			mLastKnownPosition = mPlayerPos;
			bHasLastKnownPosition = true;
			mCStatus = ChaseStatus::TargetLost;
		}
		break;

		// ステータスがTargetLostの場合
	case ChaseStatus::TargetLost:
		// プレイヤーを見失っても索敵は続ける
		// SearchPlayerに自分の座標と前ベクトルを渡す
		if (SearchPlayer())
		{
			// プレイヤーを見つけたらステータスをChaseに変更
			// UE_LOG(LogTemp, Log, TEXT("Player detected, switching to Chase state."));
			mCStatus = ChaseStatus::Chase;
		}
		// プレイヤーを見失った状態が続いている場合
		else
		{
			// プレイヤーが最後にいた位置を記録できているなら
			if (bHasLastKnownPosition)
			{
				// 目標までの距離をベクトルで算出
				float DistXY = FVector::Dist(
					FVector(mLastKnownPosition.X, mLastKnownPosition.Y, 0),
					FVector(GetActorLocation().X, GetActorLocation().Y, 0));

				// 目標までの位置との距離が100.0f以下になったら
				if (DistXY <= 100.0f)
				{
					// 最後にプレイヤーを見つけた位置の記録を消す
					// UE_LOG(LogTemp, Log, TEXT("Reached last known position."));
					bHasLastKnownPosition = false;
					// 見失った時間を0.0fに設定
					mLostnowTime = 0.0f;
				}
				// 記録していた位置との距離が100.0f以上の場合は
				else
				{
					// 記録したプレイヤーの位置に移動する
					// UE_LOG(LogTemp, Log, TEXT("Moving to last known position: %s"), *mLastKnownPosition.ToString());
					FindAndMoveToLocation(mLastKnownPosition);
				}
			}
			// プレイヤーが最後にいた位置を記録できていないなら
			else
			{
				// 見失ってからの時間を計測
				mLostnowTime += DeltaTime;

				// 見失ってからの時間が設定してある見失うまでの時間を超えたら
				if (mLostnowTime < mLostTime)
				{
					mRRInfo.mLookAroundChangeTimer -= DeltaTime;

					if (mRRInfo.mLookAroundChangeTimer <= 0.0f)
					{
						mRRInfo.mLookAroundDirection *= -1;
						mRRInfo.mTargetLookAroundYaw = FMath::FRandRange(20.0f, 60.0f);
						mRRInfo.mLookAroundChangeTimer = mRRInfo.mLookAroundChangeInterval;
					}

					FRotator CurrentRotation = GetActorRotation();
					float DeltaYaw = mRRInfo.mLookAroundDirection * mRRInfo.mTargetLookAroundYaw * DeltaTime;

					CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, CurrentRotation.Yaw + DeltaYaw, DeltaTime, mRotationSpeed);
					SetActorRotation(CurrentRotation);
				}
				else
				{
					// 完全に見失ったら初期位置に戻る
					// UE_LOG(LogTemp, Log, TEXT("Target lost completely, returning to initial position."));
					FindAndMoveToLocation(mInitPos);

					// 目標までの距離をベクトルで算出
					float DistXY = FVector::Dist(
						FVector(mInitPos.X, mInitPos.Y, 0),
						FVector(GetActorLocation().X, GetActorLocation().Y, 0));

					// 目標までの位置との距離が150.0f以下になったら
					if (DistXY <= 150.0f)
					{
						// 見失った時間を0.0fに設定し、ステータスをIdleに戻す
						// UE_LOG(LogTemp, Log, TEXT("Returned to initial position, switching to Idle state."));
						mLostnowTime = 0.0f;
						mCStatus = ChaseStatus::Idle;
					}
				}
			}
		}

		break;

	default:
		break;
	}
}

// Bossの攻撃
void ABossEnemy::BossAttack()
{
	if (!SearchPlayer())
	{
		UE_LOG(LogTemp, Log, TEXT("Boss:No player detected, BossAttack aborted."));
		if(&GetWorldTimerManager() && &mAttackTimerHandle)
		//// 次の攻撃を予約
		GetWorldTimerManager().SetTimer(mAttackTimerHandle, this, &ABossEnemy::BossAttack, 3.0f, false);
		return;
	}

	// プレイヤーとの距離を計算
	float Distance = FVector::Dist(
		FVector(GetActorLocation().X, GetActorLocation().Y, 0),
		FVector(mPlayerPos.X, mPlayerPos.Y, 0));

	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attack"));
	}
	// 呼ばれるたびに出力される
	UE_LOG(LogTemp, Log, TEXT("BossAttack."));

	// デフォルトのクールタイム
	float CooldownTime = 3.0f;

	// 攻撃
	if (Distance <= 600.0f) // 3m以内
	{
		UE_LOG(LogTemp, Log, TEXT("Boss:Performing CloseAttack (Distance: %f)"), Distance);
		CloseAttack();
		CooldownTime = 2.0f;
	}
	else if (Distance > 600.0f && Distance <= 4000.0f) // 3mより遠く、40m以内
	{

		if (SearchPlayer())
		{
			mShootingTime += GetWorld()->GetDeltaSeconds();
			if (mShootingTime >= mShootingCoolTime)
			{
				if (mOverlapOtherActor || mOverlapOtherComp)
				{
					FVector ShootLocation = GetActorLocation();
					FVector UpperLocation = ShootLocation + FVector(0.0f, 0.0f, UpPosZ);

					FVector ShootingPos = GetActorLocation() + (GetActorForwardVector() + 2.0f);
					mShootiongAttack->Attack(Power, mOverlapOtherActor, UpperLocation);
					mShootingTime = 0.0f;
				}
			}
		}

		//UE_LOG(LogTemp, Log, TEXT("Boss:Performing RushAttack (Distance: %f)"), Distance);
		// RushAttack();、
		//CooldownTime = 5.0f;
		// if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("A2"));
		// }
		//  呼ばれるたびに出力される
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Boss:No attack, player too far (Distance: %f)"), Distance);
		CooldownTime = 3.0f;
	}

	// 次の攻撃のために BossAttack を再度呼び出す（クールダウンなどを考慮）
	GetWorldTimerManager().SetTimer(mAttackTimerHandle, this, &ABossEnemy::ResetAttackCooldown, CooldownTime, false);
}

// クールタイムがリセットされたら次の攻撃
void ABossEnemy::ResetAttackCooldown()
{
	UE_LOG(LogTemp, Log, TEXT("Boss:Attack cooldown reset."));
	BossAttack(); // 次の攻撃を実行
}

// 近接攻撃
void ABossEnemy::CloseAttack()
{
	if (mIsActiveAttack)
		return;

	UE_LOG(LogTemp, Log, TEXT("Boos:Executing Close Attack"));

	mIsActiveAttack = true;

	UE_LOG(LogTemp, Log, TEXT("Boss:mIsActiveAttack %s"), mIsActiveAttack ? TEXT("True") : TEXT("False"));

	// 近接攻撃範囲を有効化
	mCloseAttackComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mCloseAttackComp->SetGenerateOverlapEvents(true);

	bool bOverlapAttack = bIsTargetOverlappingAttack;
	UE_LOG(LogTemp, Log, TEXT("Boss:OverlappingAttack: %s"), bIsTargetOverlappingAttack ? TEXT("True") : TEXT("False"));

	// 攻撃判定
	if (bIsTargetOverlappingAttack)
	{
		/*bool bOverlapAttack = bIsTargetOverlappingAttack;
		UE_LOG(LogTemp, Log, TEXT("SearchPlayer result: %s"), bOverlapAttack ? TEXT("True") : TEXT("False"));*/

		if (mOverlapOtherActor && mOverlapOtherComp)
		{
			AttackFunction(mOverlapOtherActor, mOverlapOtherComp);
			UE_LOG(LogTemp, Log, TEXT("Boss:Damage to the Player."));
		}
	}

	// アニメーションを再生
	UE_LOG(LogTemp, Log, TEXT("Boss:CloseAttackAnimation."));

	// 1秒後に EndAttack() を呼ぶ
	GetWorldTimerManager().SetTimer(mAttackEndTimerHandle, this, &ABossEnemy::EndAttack, 1.0f, false);
}

// 突進攻撃
void ABossEnemy::RushAttack()
{
	if (mIsActiveAttack)
		return;

	// if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,  // 一意の ID（-1 なら常に新しいメッセージとして表示）
	//		2.0f,  // 表示時間（秒）
	//		FColor::Black,  // テキストの色
	//		FString::Printf(TEXT("Distance: %f"), "ExecutingRushAttack")  // 出力するテキスト
	//	);
	// }

	UE_LOG(LogTemp, Log, TEXT("Boss:Executing Rush Attack"));

	mIsActiveAttack = true;

	// 突進攻撃の当たり判定を有効化
	mAttackComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mAttackComp->SetGenerateOverlapEvents(true);

	// 突進開始時のターゲット位置を保存
	mRushTargetLocation = mPlayerPos; // 事前に取得したプレイヤーの位置

	// if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("korekarakaiten"));
	// }

	// プレイヤーに向く
	// ターゲット方向を向くように回転（Yaw のみ変更）
	FVector CurrentLocation = GetActorLocation();
	FRotator CurrentRotation = GetActorRotation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, PlayerPosition);
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), mRotationSpeed);
	SetActorRotation(FRotator(CurrentRotation.Pitch, NewRotation.Yaw, CurrentRotation.Roll)); // Yaw のみ更新

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("korekarakougeki"));
	}

	//// 突進の方向を計算
	// FVector Direction = (mRushTargetLocation - GetActorLocation()).GetSafeNormal();
	// LaunchCharacter(Direction * mRushSpeed, true, true);
	// AddMovementInput(Direction, 1.0f);

	FVector TargetLocation = CurrentLocation + GetActorForwardVector() * 4.0f;
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	GetCharacterMovement()->Velocity = Direction * mMoveSpeed;
	//----------------------------------------------------------------------------------------------
	// こいつが機能していない

	// 目標地点に到達したか確認する
	GetWorldTimerManager().SetTimer(mRushCheckTimerHandle, this, &ABossEnemy::CheckRushEnd, 0.1f, true);
}

// 壁にぶつかる
// void ABossEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	// 壁にぶつかったら突進を停止
//	if (OtherActor && OtherActor->ActorHasTag("Walkable"))
//	{
//		UE_LOG(LogTemp, Log, TEXT("Rush Attack Stopped: Hit %s"), *OtherActor->GetName());
//		// 突進停止
//		GetCharacterMovement()->Velocity = FVector::ZeroVector;
//		EndAttack(); // 突進終了処理を呼ぶ
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Wallbutukaru"));
//		}
//
//	}
//}

// 攻撃終了関数
void ABossEnemy::EndAttack()
{
	// if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AttackEnded"));
	// }

	UE_LOG(LogTemp, Log, TEXT("Boss:Attack Ended"));

	// 近接攻撃の当たり判定を無効化
	mCloseAttackComp->SetGenerateOverlapEvents(false);
	mCloseAttackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 突進攻撃の当たり判定を無効化
	mAttackComp->SetGenerateOverlapEvents(false);
	mAttackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 攻撃中フラグをOFF
	mIsActiveAttack = false;
}

// 突進終了
void ABossEnemy::CheckRushEnd()
{

	float Distance = FVector::Dist2D(GetActorLocation(), mRushTargetLocation);

	//// 画面上に Distance を表示
	// if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,  // 一意の ID（-1 なら常に新しいメッセージとして表示）
	//		2.0f,  // 表示時間（秒）
	//		FColor::Yellow,  // テキストの色
	//		FString::Printf(TEXT("Distance: %f"), Distance)  // 出力するテキスト
	//	);
	// }

	//// UE_LOG でも Distance を出力
	// UE_LOG(LogTemp, Log, TEXT("Current Distance: %f"), Distance);

	if (Distance <= 50.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("Boss:Rush Attack Completed"));

		GetCharacterMovement()->Velocity = FVector::ZeroVector;

		// 突進終了処理
		EndAttack();

		// タイマーをクリア
		GetWorldTimerManager().ClearTimer(mRushCheckTimerHandle);
	}
}

// AttackCompに当たった時に呼ばれる関数
void ABossEnemy::OnOverlapAttack(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		UE_LOG(LogTemp, Log, TEXT("Boss:Hit Player, Comp name:%s"), *OtherActor->GetName());
		bIsTargetOverlappingAttack = true;
		mOverlapOtherActor = OtherActor;
		mOverlapOtherComp = OtherComp;
	}
}

// AttackCompから抜けた時に呼ばれる関数
void ABossEnemy::OnOverlapAttackEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		UE_LOG(LogTemp, Log, TEXT("Boss:Hit Player End"));
		bIsTargetOverlappingAttack = false;
		mOverlapOtherActor = nullptr;
		mOverlapOtherComp = nullptr;
	}
}

// FlightTypeEnemyの攻撃処理流用
void ABossEnemy::AttackFunction(AActor *OtherActor, UPrimitiveComponent *OtherComp)
{
	if (!OtherActor || !OtherComp)
	{
		UE_LOG(LogTemp, Log, TEXT("Boss:OverlapOtherActor,OverlapOtherComp is null"));
		return;
	}

	if (OtherActor->ActorHasTag("Player"))
	{
		TObjectPtr<UConventionalAttack> AttackObj = NewObject<UConventionalAttack>();
		AttackObj->Attack(Power, OtherActor);
	}
}

FString ABossEnemy::GetStatus()
{
	{
		float Dist = FVector::Dist(FVector(GetActorLocation().X, GetActorLocation().Y, 0)
			, FVector(mPlayerPos.X, mPlayerPos.Y, 0));
		if (Dist <= 600.0f) return("Attack");

		switch (mCStatus)
		{
		case ChaseStatus::None:
			return ("None");
		case ChaseStatus::TargetLost:
			return ("TargetLost");
		case ChaseStatus::Idle:
			return ("Idle");
		case ChaseStatus::Chase:
			return ("Chase");
		default:
			return ("");
		}
	}
}

// 攻撃処理(Flightより)
// void ABossEnemy::PlayAttack()
//{
//	// アニメーションBPのほうにて呼び出されていなければ
//	if (!mIsActiveAttack)
//	{
//		// 当たり判定を無効化
//		mAttackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//		mAttackComp->SetGenerateOverlapEvents(false);
//		return;
//	}
//	else
//	{
//		// 当たり判定を有効化
//		mAttackComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//		mAttackComp->SetGenerateOverlapEvents(true);
//
//		// 攻撃方法は突進なので
//		// 直線で前方向に素早く移動を行う
//		float speed = (mMoveSpeed * GetWorld()->GetDeltaSeconds()) * 2;
//		FVector NewPos = FVector(GetActorLocation() + (GetActorForwardVector() * speed));
//		SetActorLocation(NewPos);
//	}
//
//	// 攻撃可能範囲を確認
//	float Dist = FVector::Dist(FVector(GetActorLocation().X, GetActorLocation().Y, 0)
//		, FVector(mPlayerPos.X, mPlayerPos.Y, 0));
//	mbCanAttack = mbCanAttackRadius > Dist;
//}

// プレイヤーとの距離を計算
// float ABossEnemy::Calculatedistance()
//{
//	// 現在の位置を取得
//	FVector CurrentLocation = GetActorLocation();
//
//	// プレイヤーの位置を取得
//	FVector PlayerPos = PPlayer->GetActorLocation();
//
//	// ターゲット位置までの距離を計算
//	float Distance = FVector::Dist(PlayerPos, CurrentLocation);
//
//	return 0.0f;// Distance;
//}

/*/ Called to bind functionality to input
void ABossEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}*/
