// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// ABeybladeBase.cpp
// ベイブレードの基本クラスの実装
// 作成日：2025/03/04

#include "BeybladeBase.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/GameInstance.h"
#include "BeybladeGameInstance.h"
#include "BeybladeGameModeBase.h"
#include "EnvironmentManager.h"
#include "beyblade_prototype/TGU_AxisLocker.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// デフォルト値を設定します
ABeybladeBase::ABeybladeBase()
{
    // このキャラクターが毎フレームTick()を呼び出すように設定します。パフォーマンス向上のために不要な場合はオフにできます。
    PrimaryActorTick.bCanEverTick = true;

    // ルートコンポーネントをカプセルコンポーネントに設定
    RootComponent = BeybladeCapsuleComponent = GetCapsuleComponent();

    MeshPitchAndRollComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MeshPitchAndRoll"));
    MeshPitchAndRollComponent->SetupAttachment(RootComponent);

    // StaticMeshComponentコンポーネントを作成します
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMeshComponent->SetupAttachment(MeshPitchAndRollComponent);

    // TGU_AxisLockerコンポーネントを作成します
    AxisLocker = CreateDefaultSubobject<UTGU_AxisLocker>(TEXT("TGU_AxisLocker"));
    AxisLocker->SetupAttachment(RootComponent);

    // NiagaraComponentコンポーネントを作成します
    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    NiagaraComponent->SetupAttachment(StaticMeshComponent);

    // SplineComponentコンポーネントを作成します
    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    SplineComponent->SetupAttachment(StaticMeshComponent);

    // 予測軌道表示用のメッシュアセットを探してロードします
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PredictionMeshAsset(TEXT("StaticMesh'/Game/LevelPrototyping/Meshes/SM_Tube.SM_Tube'"));
    if (PredictionMeshAsset.Succeeded())
    {
        PredictionMesh = PredictionMeshAsset.Object;
    }

    // 予測軌道表示用のマテリアルアセットを探してロードします
    static ConstructorHelpers::FObjectFinder<UMaterial> PredictionMaterialAsset(TEXT("/Script/Engine.Material'/Game/PredictionMat.PredictionMat'"));
    if (PredictionMaterialAsset.Succeeded())
    {
        PredictionMat = PredictionMaterialAsset.Object;
    }

    // Niagaraエフェクトのアセットを探してロードします
    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HitEffectAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/Characters/beyblade/BeybladeSparks.BeybladeSparks'"));
    if (HitEffectAsset.Succeeded())
    {
        HitEffectSystemClass = HitEffectAsset.Object;
    }
}

// ゲーム開始時またはスポーン時に呼ばれます
void ABeybladeBase::BeginPlay()
{
    Super::BeginPlay();

    // CapsuleComponentのワールド位置を取得し、待機位置として設定します
    StandbyPos = GetCapsuleComponent()->GetComponentLocation();

    // MaxHP変数を設定します
    MaxHP = HP;

    // GameInstanceを取得し、BeybladeGameInstanceにキャストします
    UBeybladeGameInstance *BeybladeGameInstance = Cast<UBeybladeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (BeybladeGameInstance)
    {
        bbGameInstance = BeybladeGameInstance;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast to UBeybladeGameInstance!"));
    }

    // Niagaraエフェクトを非アクティブ化します
    NiagaraComponent->SetActive(false);

    // スタティックメッシュのコリジョンを無効にします
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    MeshPitchAndRollComponent->SetWorldLocation(BeybladeCapsuleComponent->GetComponentLocation() + FVector(0.0f, 0.0f, -6.5f)); // メッシュのピッチとロールコンポーネントの位置を初期化します

    // コンポーネントヒット時のイベントに関数をバインドします
    BeybladeCapsuleComponent->OnComponentHit.AddDynamic(this, &ABeybladeBase::OnComponentHit);
    BeybladeCapsuleComponent->SetUseCCD(true); // 連続衝突検出を有効にします

    // 必殺技発動の遅延時間を初期化します
    UltimateCastingDelay = 0.0f;

    // コントローラーを確認します
    ControllerType = EBeybladeControllerType::None;
    CheckController();

    // OnTakeAnyDamageデリゲートにAnyDamage関数をバインドします
    OnTakeAnyDamage.AddDynamic(this, &ABeybladeBase::AnyDamage);
}

// ダメージを受けた際の処理
void ABeybladeBase::AnyDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType, AController *InstigatedBy, AActor *DamageCauser)
{
    // ダメージがあり、ダメージクールダウンが終わっている場合
    if (Damage > 0.0f && DamageCDTimer <= 0.0f)
    {
        // ダメージ軽減を考慮してHPを減らします
        HP -= Damage * DamageReduction;

        // ダメージクールダウンタイマーを設定します
        DamageCDTimer = DamageCD;
    }
}

// コントローラーの割り当てを確認し、設定する
void ABeybladeBase::CheckController()
{
    // コントローラーインデックスが割り当てられていない場合は何もしない
    if (bIsControllerAssigned)
    {
        return;
    }

    if (bbGameInstance && bbGameInstance->BeybladeList.Num() == 2)
    {
        // ベイブレードインデックスを取得
        BeybladeIndex = *bbGameInstance->BeybladeList.FindKey(this);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("BeybladeGameInstance is not valid!"));
        return;
    }

    // コントローラータイプが未割り当ての場合、タイプを取得します
    if (ControllerType == EBeybladeControllerType::None)
    {
        // インデックスは１の場合はプレイヤー1
        if (BeybladeIndex == 0)
        {
            ControllerType = EBeybladeControllerType::P1;
        }

        else // インデックスは２の場合はプレイヤー2かCPU
        {
            // ゲームモードを取得
            ABeybladeGameModeBase *GameMode = Cast<ABeybladeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
            const FString ModeStr = GameMode->GetModeStr();

            // 対戦モードの場合
            if (ModeStr != "CPU")
            {
                ControllerType = EBeybladeControllerType::P2;
            }
            else // CPUモードの場合
            {
                ControllerType = EBeybladeControllerType::AI;
            }
        }
    }

    // 既にコントローラーが割り当てられている場合は処理を中断
    if (Controller)
    {
        bIsControllerAssigned = true;
        return;
    }

    // コントローラータイプに応じて処理を分岐
    if (ControllerType == EBeybladeControllerType::AI)
    {
        // AIコントローラーを取得します
        AAIController *AIController = Cast<AAIController>(UGameplayStatics::GetActorOfClass(GetWorld(), AAIController::StaticClass()));
        if (AIController)
        {
            // AIコントローラーが見つかった場合、ポゼッションを行います
            AIController->Possess(this);
            bIsControllerAssigned = true;
        }

        // 以降の処理はAIコントローラーの場合は行わない
        return;
    }

    Controller = UGameplayStatics::GetPlayerController(GetWorld(), BeybladeIndex);
    if (Controller)
    {
        Controller->Possess(this);
        bIsControllerAssigned = true;
    }

    if (ControllerType == EBeybladeControllerType::P1)
    {
        // シュート角度の最大値を設定
        ShootAngleMax = 1.7;

        // シュート角度の最小値を設定
        ShootAngleMin = 0.0f;
    }
    else if (ControllerType == EBeybladeControllerType::P2)
    {
        // シュート角度の最大値を設定
        ShootAngleMax = 0;

        // シュート角度の最小値を設定
        ShootAngleMin = -1.7;
    }
}

// 毎フレーム呼ばれます
void ABeybladeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    InternalDeltaTime = DeltaTime; // 内部で使用するデルタタイムを更新

    // コントローラーが未割り当ての場合、確認処理を実行
    if (!bIsControllerAssigned)
    {
        CheckController();
    }

    // プレイヤー操作の場合の処理
    if (ControllerType == EBeybladeControllerType::P1 || ControllerType == EBeybladeControllerType::P2)
    {
        // ステータスが通常または必殺技中の場合、シュート済みフラグを立てる
        bIsShot = (Status == EBeybladeStatus::Ready) ? false : true;
    }
    // else // AI操作の場合の処理 ※AIコントローラ実装するためにコメントアウト
    // {
    //     // AIのシュートパワーを設定
    //     LaunchPower = 500.0f;

    //     // AIの照準ベクトルを設定
    //     AimVector = FVector(-1.7f, 0.0f, 0.0f);

    //     // プレイヤー0のポーンがシュート済みか確認し、自身のシュート済みフラグに反映
    //     if (APlayerController *PlayerController0 = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    //     {
    //         if (APawn *ControlledPawn0 = PlayerController0->GetPawn())
    //         {
    //             bIsShot = Cast<ABeybladeBase>(ControlledPawn0)->IsShot();
    //         }
    //     }
    // }

    // シュート後の共通処理
    if (bIsShot)
    {
        // // 予測軌道を削除　※AIコントローラ実装するためにコメントアウト
        // DeletePrediction();

        // // AIが準備完了状態ならシュートを実行
        // if (ControllerType == EBeybladeControllerType::AI)
        // {
        //     if (Status == EBeybladeStatus::Ready)
        //     {
        //         LaunchTriggered();
        //     }
        // }

        // Niagaraエフェクトが非アクティブならアクティブにする
        if (NiagaraComponent && !NiagaraComponent->IsActive())
        {
            NiagaraComponent->Activate(false);
        }

        // HPが0以下なら停止処理
        if (HP <= 0.0)
        {
            Stop();
        }

        if (Status != EBeybladeStatus::Stopped)
        {
            // ブースト中ならトレイルの色を赤に
            if (NiagaraComponent && IsBoost)
            {
                NiagaraComponent->SetNiagaraVariableLinearColor(TEXT("TrailColor"), FLinearColor::Red);
            }
            else // 非ブースト中はブーストゲージを回復し、トレイルの色を白に
            {
                Boost = FMath::Clamp(Boost + BoostRegen * DeltaTime, 0.0f, BoostMax);
                NiagaraComponent->SetNiagaraVariableLinearColor(TEXT("TrailColor"), FLinearColor::White);
            }
        }
    }

    // 準備完了状態の場合
    if (Status == EBeybladeStatus::Ready)
    {
        // 予測軌道を更新
        UpdatePrediction();

        // 待機位置にベイブレードを固定
        if (BeybladeCapsuleComponent)
        {
            BeybladeCapsuleComponent->SetWorldLocation(StandbyPos, false, nullptr, ETeleportType::None);
        }
    }

    // 停止も準備中でもない場合（＝戦闘中）
    else if ((Status != EBeybladeStatus::Stopped) && (Status != EBeybladeStatus::Ready))
    {
        // スタティックメッシュの位置と回転を更新
        if (StaticMeshComponent)
        {
            // カプセルコンポーネントの位置に合わせてメッシュの位置を調整
            FVector NewLocation = BeybladeCapsuleComponent->GetComponentLocation() + FVector(0.0f, 0.0f, -6.5f);

            // 現在の回転を取得
            FRotator CurrentRotation = StaticMeshComponent->GetComponentRotation();
            // 回転速度を加算して新しいYawを計算
            float newRoll = FMath::Clamp(GetVelocity().X * 0.3f, -45.0f, 45.0f);
            float newPitch = FMath::Clamp(GetVelocity().Y * 0.3f, -45.0f, 45.0f);
            float NewYaw = CurrentRotation.Yaw + RotationSpeed * DeltaTime;
            // 新しい位置と回転を設定
            MeshPitchAndRollComponent->SetWorldLocationAndRotation(NewLocation, FRotator(newPitch, 0, newRoll), false, nullptr, ETeleportType::None);
            StaticMeshComponent->SetWorldRotation(FRotator(0.0f, NewYaw, 0.0f), false, nullptr, ETeleportType::None);
        }

        // ダメージクールダウンタイマーを更新
        if (DamageCDTimer > 0.0)
        {
            DamageCDTimer = FMath::Clamp(DamageCDTimer - DeltaTime, 0, DamageCD);
        }

        // 相手がいる場合、HPを自然減少させる
        if (Status != EBeybladeStatus::Ready && bbGameInstance->BeybladeList.Num() > 1)
        {
            HP -= DurabilityCoefficient * DeltaTime;
        }
    }

    // 必殺技発動遅延中の処理
    if (UltimateCastingDelay > 0.0f)
    {
        UltimateCastingDelay = FMath::Clamp((UltimateCastingDelay - DeltaTime), 0.0f, 5.0f);
        // 遅延が終わったら必殺技を発動
        if (UltimateCastingDelay <= 0.0f)
        {
            bbGameInstance->EnvironmentManager->ToggleUltraLightEffect(false);
            Status = EBeybladeStatus::Ultra;
            IsUltimateUsed = true;
            ActiveUltimate();
        }
    }

    // 必殺技中の処理
    if (Status == EBeybladeStatus::Ultra)
    {
        UltimateTick(DeltaTime);
    }
}

// 入力機能と処理をバインドします
void ABeybladeBase::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 入力アクションアセットをロード
    BoostInput = LoadObject<UInputAction>(NULL, TEXT("/Game/IA_Boost"), NULL, LOAD_None, NULL);
    CameraInput = LoadObject<UInputAction>(NULL, TEXT("/Game/IA_Camera"), NULL, LOAD_None, NULL);
    LaunchInput = LoadObject<UInputAction>(NULL, TEXT("/Game/IA_Launch"), NULL, LOAD_None, NULL);
    MoveInput = LoadObject<UInputAction>(NULL, TEXT("/Game/IA_Move"), NULL, LOAD_None, NULL);
    UltraInput = LoadObject<UInputAction>(NULL, TEXT("/Game/IA_Ultra"), NULL, LOAD_None, NULL);
    PauseInput = LoadObject<UInputAction>(NULL, TEXT("/Game/IA_Pause"), NULL, LOAD_None, NULL);

    UEnhancedInputComponent *Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    // ETriggerEvent の値を変えることで、任意のトリガーイベントにバインドできます

    // 各入力アクションと対応する関数をバインド
    Input->BindAction(BoostInput, ETriggerEvent::Triggered, this, &ABeybladeBase::BoostTriggered);
    Input->BindAction(BoostInput, ETriggerEvent::Completed, this, &ABeybladeBase::BoostCompleted);
    Input->BindAction(UltraInput, ETriggerEvent::Triggered, this, &ABeybladeBase::UltraTriggered);
    Input->BindAction(LaunchInput, ETriggerEvent::Triggered, this, &ABeybladeBase::LaunchTriggered);
    Input->BindAction(CameraInput, ETriggerEvent::Triggered, this, &ABeybladeBase::CameraTriggered);
    Input->BindAction(MoveInput, ETriggerEvent::Triggered, this, &ABeybladeBase::MoveTriggered);
    Input->BindAction(PauseInput, ETriggerEvent::Triggered, this, &ABeybladeBase::PauseTriggered);
}

// ブースト入力がトリガーされた時の処理
void ABeybladeBase::BoostTriggered()
{
    // 戦闘中でない場合は処理しない
    if (Status != EBeybladeStatus::Stopped && Status != EBeybladeStatus::Ready)
    {
        bool canUseBoost = false;
        // ブースト中かつブーストゲージが残っている場合
        if (IsBoost)
        {
            if (Boost > 0.0f)
            {
                canUseBoost = true;
            }
        }
        else // 非ブースト中で、ブーストゲージが最低値以上ある場合
        {
            if (GetBoostPercentage() > BoostCost * InternalDeltaTime)
            {
                canUseBoost = true;
            }
        }

        // ブーストが使用可能なら、ゲージを消費してブーストフラグを立てる
        if (canUseBoost)
        {
            Boost = FMath::Clamp((Boost - (BoostCost * InternalDeltaTime)), 0.0, 100.0);
            IsBoost = true;
        }
        else // 使用不可ならブーストフラグを下ろす
        {
            IsBoost = false;
        }
    }
}

// 移動入力がトリガーされた時の処理
void ABeybladeBase::MoveTriggered(const FInputActionInstance &Instance)
{
    // 戦闘中でない場合は処理しない
    if ((Status == EBeybladeStatus::Stopped) || (Status == EBeybladeStatus::Ready))
    {
        return;
    }

    // ダメージクールダウン中は処理しない
    if (DamageCDTimer > 0.0f)
    {
        return;
    }

    // トルク値を計算
    FVector2D ActionValue = Instance.GetValue().Get<FVector2D>();
    FVector2D NormalizedActionValue = UKismetMathLibrary::NormalSafe2D(ActionValue);

    // 移動値がゼロでない場合、Move関数を呼び出す
    if (!NormalizedActionValue.IsNearlyZero())
    {
        Move(NormalizedActionValue);
    }
}

void ABeybladeBase::Move(const FVector2D &MoveValue)
{
    // ブースト状態に応じて速度係数を選択
    float SpeedMultiplier = IsBoost ? 1.5 : 1.0;

    // トルクを計算
    float TorqueX = MoveValue.X * MoveSpeed * SpeedMultiplier * InternalDeltaTime;
    float TorqueY = MoveValue.Y * MoveSpeed * SpeedMultiplier * InternalDeltaTime;

    // CapsuleComponentにトルク（ラジアン単位）を追加
    BeybladeCapsuleComponent->AddTorqueInRadians(FVector(TorqueX, TorqueY, 0.0), NAME_None, false);
}

// ブースト入力が完了した時の処理
void ABeybladeBase::BoostCompleted()
{
    IsBoost = false;
}

// シュート入力がトリガーされた時の処理
void ABeybladeBase::LaunchTriggered()
{
    // 準備完了状態でない場合は処理しない
    if (Status != EBeybladeStatus::Ready)
    {
        return;
    }

    // ステータスを「通常」に変更
    Status = EBeybladeStatus::Normal;

    if (BeybladeCapsuleComponent)
    {
        // コリジョンと物理シミュレーションを有効化
        BeybladeCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        BeybladeCapsuleComponent->SetSimulatePhysics(true);

        // 照準方向にパワーを乗せて力を加える
        FVector Force = (AimVector * LaunchPower);
        BeybladeCapsuleComponent->AddForce(Force, NAME_None, true);
    }

    // 予測軌道を削除
    DeletePrediction();

    // スプラインコンポーネントを破棄
    if (SplineComponent)
    {
        SplineComponent->DestroyComponent(false);
    }

    // Niagaraエフェクトを有効化
    if (NiagaraComponent)
    {
        NiagaraComponent->SetActive(true);
    }
}

// ポーズ入力がトリガーされた時の処理
void ABeybladeBase::PauseTriggered()
{
    // ゲームモードを取得
    ABeybladeGameModeBase *GameMode = Cast<ABeybladeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

    // ゲームが終了していなければポーズを切り替える
    if (GameMode->GetGameState() != EGameState::Finished)
    {
        GameMode->TogglePause(int(ControllerType));
    }
    else // ゲームが終了していればレベルをリロード
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), false);
    }
}

// カメラ(照準)入力がトリガーされた時の処理
void ABeybladeBase::CameraTriggered(const FInputActionInstance &Instance)
{
    // 入力値を取得
    float ActionValueX = Instance.GetValue().Get<FVector2D>().X;
    float ActionValueY = Instance.GetValue().Get<FVector2D>().Y;

    // 照準ベクトルを計算（角度制限を適用）
    float AimVectorX = FMath::Clamp(AimVector.X + ActionValueY * AimSpeed, ShootAngleMin, ShootAngleMax);
    float AimVectorY = FMath::Clamp(AimVector.Y + ActionValueX * AimSpeed, -0.7, 0.7);

    // 照準ベクトルを更新
    AimVector = FVector(AimVectorX, AimVectorY, 0.0f);
}

// 2つのベクトル間の角度を計算します
float ABeybladeBase::FindAngleBetweenTwoVectors(FVector VecA, FVector VecB)
{
    // ベクトルを正規化
    FVector NormalizedA = VecA.GetSafeNormal(0.0001f);
    FVector NormalizedB = VecB.GetSafeNormal(0.0001f);
    // 内積を計算
    float DotProduct = FVector::DotProduct(NormalizedA, NormalizedB);
    // 内積から角度（度数法）を計算して返す
    return FMath::RadiansToDegrees(FMath::Acos(DotProduct));
}

// 予測軌道を更新します
void ABeybladeBase::UpdatePrediction()
{
    // 既存の予測軌道を削除
    DeletePrediction();

    if (Controller)
    {
        // 予測パラメータを設定
        FPredictProjectilePathParams PredictParams;
        PredictParams.StartLocation = BeybladeCapsuleComponent->GetComponentLocation();
        PredictParams.LaunchVelocity = AimVector * 200.0f; // 予測用の速度を調整
        PredictParams.bTraceWithCollision = true;
        PredictParams.ProjectileRadius = 6.8f;
        PredictParams.MaxSimTime = 1.0f;
        PredictParams.bTraceWithChannel = true;
        PredictParams.TraceChannel = ECC_WorldStatic;
        PredictParams.ActorsToIgnore.Add(this);
        PredictParams.SimFrequency = 20.0f;
        PredictParams.OverrideGravityZ = 0.0f; // 重力の影響を無視
        PredictParams.DrawDebugType = EDrawDebugTrace::None;
        PredictParams.DrawDebugTime = 1.0f;
        PredictParams.bTraceComplex = false;

        // 弾道予測を実行
        FPredictProjectilePathResult PredictResult;
        if (UGameplayStatics::Blueprint_PredictProjectilePath_Advanced(this, PredictParams, PredictResult))
        {
            // 予測結果のパスデータをスプラインの点として設定
            TArray<FVector> SplinePoints;
            for (const FPredictProjectilePathPointData &PathData : PredictResult.PathData)
            {
                SplinePoints.Add(PathData.Location);
            }

            SplineComponent->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::World, true);

            // スプラインの各セグメントに沿ってメッシュを配置
            int32 LastIndex = SplineComponent->GetNumberOfSplineSegments() - 1;
            for (int32 Index = 0; Index <= LastIndex; ++Index)
            {
                // セグメントの開始位置/タンジェントと終了位置/タンジェントを取得
                FVector StartPos, StartTangent, EndPos, EndTangent;
                SplineComponent->GetLocationAndTangentAtSplinePoint(Index, StartPos, StartTangent, ESplineCoordinateSpace::World);
                SplineComponent->GetLocationAndTangentAtSplinePoint(Index + 1, EndPos, EndTangent, ESplineCoordinateSpace::World);

                // スプラインメッシュコンポーネントを新規作成
                FString SegmentName = "SplineMesh" + FString::SanitizeFloat(Index);
                USplineMeshComponent *SplineMeshComp = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass(), *SegmentName);
                SplineMeshComp->RegisterComponent();
                SplineMeshComp->SetMobility(EComponentMobility::Movable);

                // スプラインメッシュのプロパティを設定
                SplineMeshComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;
                SplineMeshComp->SetStaticMesh(PredictionMesh);
                SplineMeshComp->SetMaterial(0, PredictionMat);
                SplineMeshComp->SetStartScale(FVector2D(0.01f, 0.01f));
                SplineMeshComp->SetEndScale(FVector2D(0.01f, 0.01f));
                SplineMeshComp->SetStartRoll(0.0f);
                SplineMeshComp->SetEndRoll(0.0f);
                SplineMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                SplineMeshComp->ForwardAxis = ESplineMeshAxis::Z;
                SplineMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
                SplineMeshComponentArray.Add(SplineMeshComp);

                // スプラインメッシュの開始と終了を設定
                SplineMeshComp->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
            }
        }
    }
}

// 予測軌道（スプラインメッシュ）を削除します
void ABeybladeBase::DeletePrediction()
{
    // 配列内のすべてのスプラインメッシュコンポーネントを破棄
    for (USplineMeshComponent *Element : SplineMeshComponentArray)
    {
        if (Element)
        {
            Element->DestroyComponent();
        }
    }
    // 配列を空にする
    SplineMeshComponentArray.Empty();
}

// 敗北者をゲームインスタンスのリストから削除します
void ABeybladeBase::RemoveLoser()
{
    if (bbGameInstance)
    {
        TMap<int32, ABeybladeBase *> &BeybladeList = bbGameInstance->BeybladeList;

        // リストを自身を見つけたら削除
        if (BeybladeList.Contains(BeybladeIndex) && *BeybladeList.Find(BeybladeIndex) == this)
        {
            BeybladeList.Remove(BeybladeIndex);
        }
    }
}

// 必殺技発動時の処理（基底クラスでは空、派生クラスでオーバーライド）
void ABeybladeBase::ActiveUltimate()
{
}

// ダメージを与えた時の処理（基底クラスでは空、派生クラスでオーバーライド）
void ABeybladeBase::OnDamageDealt()
{
}

// 必殺技中のTick処理
void ABeybladeBase::UltimateTick(float Deltatime)
{
    // 必殺技中でなければ処理しない
    if (Status != EBeybladeStatus::Ultra)
    {
        return;
    }

    // 必殺技タイマーを減算
    if (UltimateTimer > 0.0)
    {
        UltimateTimer -= Deltatime;
    }
    else // タイマーが0になったら通常状態に戻す
    {
        UltimateTimer = 0.0;
        Status = EBeybladeStatus::Normal;
    }
}

// ベイブレードを停止させる処理
void ABeybladeBase::Stop()
{
    Status = EBeybladeStatus::Stopped;

    // この処理が一度しか実行されないように
    if (!bHasStopExecuted)
    {
        bHasStopExecuted = true;

        // 敗北者リストから自身を削除
        RemoveLoser();

        if (StaticMeshComponent)
        {
            // 親からデタッチし、物理シミュレーションを有効化して回転させる
            StaticMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
            StaticMeshComponent->SetSimulatePhysics(true);
            StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
            StaticMeshComponent->AddTorqueInRadians(FVector(0.0f, 0.0f, 500.0f), NAME_None, true);
        }

        if (BeybladeCapsuleComponent)
        {
            // コリジョンを無効化
            BeybladeCapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
            BeybladeCapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
        }

        // AxisLockerで全軸の移動と回転をロック
        if (AxisLocker)
        {
            AxisLocker->SetNewConstraintMode(true, true, true, true, true, true);
        }

        // Niagaraエフェクトを破棄
        if (NiagaraComponent)
        {
            NiagaraComponent->DestroyComponent();
        }
    }
}

// 必殺技入力がトリガーされた時の処理
void ABeybladeBase::UltraTriggered()
{
    // タイムディレーションが1でない（スローモーション中など）または必殺技が使用不可の場合は処理しない
    if (UGameplayStatics::GetGlobalTimeDilation(GetWorld()) != 1.0f || !CanUseUltimate())
    {
        return;
    }

    // ゲームインスタンスに必殺技使用を通知
    bbGameInstance->NotifyUltraStatus(this);

    // タイムディレーションを考慮して必殺技発動までの遅延時間を設定
    UltimateCastingDelay = 3.0f / (1.0f / UGameplayStatics::GetGlobalTimeDilation(GetWorld()));
}

// コンポーネントが何かにヒットした時の処理
void ABeybladeBase::OnComponentHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    // ヒットした相手がスタティックメッシュなら処理しない
    if (Cast<UStaticMeshComponent>(OtherComp))
    {
        return;
    }

    // 自身が停止状態なら処理しない
    if (Status == EBeybladeStatus::Stopped)
    {
        return;
    }

    // ヒットした相手が他のベイブレードの場合
    if (ABeybladeBase *otherBB = Cast<ABeybladeBase>(OtherActor))
    {
        // 自身の速度を取得
        FVector Velocity = BeybladeCapsuleComponent->GetComponentVelocity();

        // 速度ベクトルと衝突法線の間の角度を計算
        float Angle = FindAngleBetweenTwoVectors(Velocity, Hit.ImpactNormal);

        // 速度の大きさを取得
        float power = Velocity.Size();

        // 角度とパワーからダメージを計算（最大ダメージでクランプ）
        float Damage = FMath::Clamp(Angle * power * DamageCoefficient, 0.0f, MaxDamage);

        // 相手にダメージを適用
        UGameplayStatics::ApplyDamage(otherBB, Damage, Controller, this, UDamageType::StaticClass());

        // 衝突位置から離れる方向のベクトルを計算
        FVector ForceDirection = (BeybladeCapsuleComponent->GetComponentLocation() - Hit.Location).GetSafeNormal(0.0001f, FVector::ZeroVector);

        // 衝突の衝撃力に基づいて反発力を計算
        float ForceMultiplier = FMath::Clamp(
            NormalImpulse.Size() * 100.0f /*衝撃力*/
                * (UKismetMathLibrary::RandomFloat() * 4.0f + 1.0f) /*ランダムな係数*/,
            1000.0f /*最小値*/, 7000.0f /*最大値*/);

        // 反発力ベクトルを計算（少し上向きの力を加える）
        FVector Force = ForceDirection * ForceMultiplier + FVector(0.0f, 0.0f, 100.0f);

        // ヒットエフェクトを生成
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitEffectSystemClass, Hit.ImpactPoint, FRotator::ZeroRotator);

        // 自身に反発力を加える
        BeybladeCapsuleComponent->AddForce(Force, NAME_None, true);
        UE_LOG(LogTemp, Warning, TEXT("ForceDirection: %s"), *ForceDirection.ToString());
    }
}

void ABeybladeBase::AddMoveInput(FVector2D MoveDirection, bool bIsBoosting)
{
    if ((Status == EBeybladeStatus::Stopped) || (Status == EBeybladeStatus::Ready) || DamageCDTimer > 0.0f)
    {
        return;
    }

    if (bIsBoosting)
    {
        BoostTriggered();
    }
    else
    {
        BoostCompleted();
    }

    // 入力値がゼロでない場合、Move関数を呼び出す
    if (!MoveDirection.IsNearlyZero())
    {
        // Move関数を呼び出して移動処理を行う
        Move(MoveDirection);
    }
}

void ABeybladeBase::PerformLaunch(FVector AimDirection, float Power)
{
    if (Status != EBeybladeStatus::Ready)
    {
        return;
    }
    Status = EBeybladeStatus::Normal;

    bIsShot = true;

    if (BeybladeCapsuleComponent)
    {
        BeybladeCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        BeybladeCapsuleComponent->SetSimulatePhysics(true);

        FVector Force = (AimDirection.GetSafeNormal() * Power);
        BeybladeCapsuleComponent->AddForce(Force, NAME_None, true);
    }

    DeletePrediction();
    if (SplineComponent)
    {
        SplineComponent->DestroyComponent(false);
    }
    if (NiagaraComponent)
    {
        NiagaraComponent->SetActive(true);
    }
}

void ABeybladeBase::SetBoostStatus(bool bNewBoostStatus)
{
    if (Status != EBeybladeStatus::Normal && Status != EBeybladeStatus::Ultra)
        return;

    if (bNewBoostStatus)
    {
        // BoostTriggered のロジックを参考にブースト可能かチェック
        bool canUseBoost = false;
        if (IsBoost)
        {
            if (Boost > 0.0f)
                canUseBoost = true;
        }
        else
        {
            if (GetBoostPercentage() > 0.1f)
                canUseBoost = true;
        }
        IsBoost = canUseBoost;
    }
    else
    {
        IsBoost = false;
    }
}