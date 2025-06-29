// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// ABeybladeBase.cpp
// �x�C�u���[�h�̊�{�N���X�̎���
// �쐬���F2025/03/04

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

// �f�t�H���g�l��ݒ肵�܂�
ABeybladeBase::ABeybladeBase()
{
    // ���̃L�����N�^�[�����t���[��Tick()���Ăяo���悤�ɐݒ肵�܂��B�p�t�H�[�}���X����̂��߂ɕs�v�ȏꍇ�̓I�t�ɂł��܂��B
    PrimaryActorTick.bCanEverTick = true;

    // ���[�g�R���|�[�l���g���J�v�Z���R���|�[�l���g�ɐݒ�
    RootComponent = BeybladeCapsuleComponent = GetCapsuleComponent();

    MeshPitchAndRollComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MeshPitchAndRoll"));
    MeshPitchAndRollComponent->SetupAttachment(RootComponent);

    // StaticMeshComponent�R���|�[�l���g���쐬���܂�
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMeshComponent->SetupAttachment(MeshPitchAndRollComponent);

    // TGU_AxisLocker�R���|�[�l���g���쐬���܂�
    AxisLocker = CreateDefaultSubobject<UTGU_AxisLocker>(TEXT("TGU_AxisLocker"));
    AxisLocker->SetupAttachment(RootComponent);

    // NiagaraComponent�R���|�[�l���g���쐬���܂�
    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    NiagaraComponent->SetupAttachment(StaticMeshComponent);

    // SplineComponent�R���|�[�l���g���쐬���܂�
    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    SplineComponent->SetupAttachment(StaticMeshComponent);

    // �\���O���\���p�̃��b�V���A�Z�b�g��T���ă��[�h���܂�
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PredictionMeshAsset(TEXT("StaticMesh'/Game/LevelPrototyping/Meshes/SM_Tube.SM_Tube'"));
    if (PredictionMeshAsset.Succeeded())
    {
        PredictionMesh = PredictionMeshAsset.Object;
    }

    // �\���O���\���p�̃}�e���A���A�Z�b�g��T���ă��[�h���܂�
    static ConstructorHelpers::FObjectFinder<UMaterial> PredictionMaterialAsset(TEXT("/Script/Engine.Material'/Game/PredictionMat.PredictionMat'"));
    if (PredictionMaterialAsset.Succeeded())
    {
        PredictionMat = PredictionMaterialAsset.Object;
    }

    // Niagara�G�t�F�N�g�̃A�Z�b�g��T���ă��[�h���܂�
    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HitEffectAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/Characters/beyblade/BeybladeSparks.BeybladeSparks'"));
    if (HitEffectAsset.Succeeded())
    {
        HitEffectSystemClass = HitEffectAsset.Object;
    }
}

// �Q�[���J�n���܂��̓X�|�[�����ɌĂ΂�܂�
void ABeybladeBase::BeginPlay()
{
    Super::BeginPlay();

    // CapsuleComponent�̃��[���h�ʒu���擾���A�ҋ@�ʒu�Ƃ��Đݒ肵�܂�
    StandbyPos = GetCapsuleComponent()->GetComponentLocation();

    // MaxHP�ϐ���ݒ肵�܂�
    MaxHP = HP;

    // GameInstance���擾���ABeybladeGameInstance�ɃL���X�g���܂�
    UBeybladeGameInstance *BeybladeGameInstance = Cast<UBeybladeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (BeybladeGameInstance)
    {
        bbGameInstance = BeybladeGameInstance;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast to UBeybladeGameInstance!"));
    }

    // Niagara�G�t�F�N�g���A�N�e�B�u�����܂�
    NiagaraComponent->SetActive(false);

    // �X�^�e�B�b�N���b�V���̃R���W�����𖳌��ɂ��܂�
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    MeshPitchAndRollComponent->SetWorldLocation(BeybladeCapsuleComponent->GetComponentLocation() + FVector(0.0f, 0.0f, -6.5f)); // ���b�V���̃s�b�`�ƃ��[���R���|�[�l���g�̈ʒu�����������܂�

    // �R���|�[�l���g�q�b�g���̃C�x���g�Ɋ֐����o�C���h���܂�
    BeybladeCapsuleComponent->OnComponentHit.AddDynamic(this, &ABeybladeBase::OnComponentHit);
    BeybladeCapsuleComponent->SetUseCCD(true); // �A���Փˌ��o��L���ɂ��܂�

    // �K�E�Z�����̒x�����Ԃ����������܂�
    UltimateCastingDelay = 0.0f;

    // �R���g���[���[���m�F���܂�
    ControllerType = EBeybladeControllerType::None;
    CheckController();

    // OnTakeAnyDamage�f���Q�[�g��AnyDamage�֐����o�C���h���܂�
    OnTakeAnyDamage.AddDynamic(this, &ABeybladeBase::AnyDamage);
}

// �_���[�W���󂯂��ۂ̏���
void ABeybladeBase::AnyDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType, AController *InstigatedBy, AActor *DamageCauser)
{
    // �_���[�W������A�_���[�W�N�[���_�E�����I����Ă���ꍇ
    if (Damage > 0.0f && DamageCDTimer <= 0.0f)
    {
        // �_���[�W�y�����l������HP�����炵�܂�
        HP -= Damage * DamageReduction;

        // �_���[�W�N�[���_�E���^�C�}�[��ݒ肵�܂�
        DamageCDTimer = DamageCD;
    }
}

// �R���g���[���[�̊��蓖�Ă��m�F���A�ݒ肷��
void ABeybladeBase::CheckController()
{
    // �R���g���[���[�C���f�b�N�X�����蓖�Ă��Ă��Ȃ��ꍇ�͉������Ȃ�
    if (bIsControllerAssigned)
    {
        return;
    }

    if (bbGameInstance && bbGameInstance->BeybladeList.Num() == 2)
    {
        // �x�C�u���[�h�C���f�b�N�X���擾
        BeybladeIndex = *bbGameInstance->BeybladeList.FindKey(this);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("BeybladeGameInstance is not valid!"));
        return;
    }

    // �R���g���[���[�^�C�v�������蓖�Ă̏ꍇ�A�^�C�v���擾���܂�
    if (ControllerType == EBeybladeControllerType::None)
    {
        // �C���f�b�N�X�͂P�̏ꍇ�̓v���C���[1
        if (BeybladeIndex == 0)
        {
            ControllerType = EBeybladeControllerType::P1;
        }

        else // �C���f�b�N�X�͂Q�̏ꍇ�̓v���C���[2��CPU
        {
            // �Q�[�����[�h���擾
            ABeybladeGameModeBase *GameMode = Cast<ABeybladeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
            const FString ModeStr = GameMode->GetModeStr();

            // �ΐ탂�[�h�̏ꍇ
            if (ModeStr != "CPU")
            {
                ControllerType = EBeybladeControllerType::P2;
            }
            else // CPU���[�h�̏ꍇ
            {
                ControllerType = EBeybladeControllerType::AI;
            }
        }
    }

    // ���ɃR���g���[���[�����蓖�Ă��Ă���ꍇ�͏����𒆒f
    if (Controller)
    {
        bIsControllerAssigned = true;
        return;
    }

    // �R���g���[���[�^�C�v�ɉ����ď����𕪊�
    if (ControllerType == EBeybladeControllerType::AI)
    {
        // AI�R���g���[���[���擾���܂�
        AAIController *AIController = Cast<AAIController>(UGameplayStatics::GetActorOfClass(GetWorld(), AAIController::StaticClass()));
        if (AIController)
        {
            // AI�R���g���[���[�����������ꍇ�A�|�[�b�V�������s���܂�
            AIController->Possess(this);
            bIsControllerAssigned = true;
        }

        // �ȍ~�̏�����AI�R���g���[���[�̏ꍇ�͍s��Ȃ�
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
        // �V���[�g�p�x�̍ő�l��ݒ�
        ShootAngleMax = 1.7;

        // �V���[�g�p�x�̍ŏ��l��ݒ�
        ShootAngleMin = 0.0f;
    }
    else if (ControllerType == EBeybladeControllerType::P2)
    {
        // �V���[�g�p�x�̍ő�l��ݒ�
        ShootAngleMax = 0;

        // �V���[�g�p�x�̍ŏ��l��ݒ�
        ShootAngleMin = -1.7;
    }
}

// ���t���[���Ă΂�܂�
void ABeybladeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    InternalDeltaTime = DeltaTime; // �����Ŏg�p����f���^�^�C�����X�V

    // �R���g���[���[�������蓖�Ă̏ꍇ�A�m�F���������s
    if (!bIsControllerAssigned)
    {
        CheckController();
    }

    // �v���C���[����̏ꍇ�̏���
    if (ControllerType == EBeybladeControllerType::P1 || ControllerType == EBeybladeControllerType::P2)
    {
        // �X�e�[�^�X���ʏ�܂��͕K�E�Z���̏ꍇ�A�V���[�g�ς݃t���O�𗧂Ă�
        bIsShot = (Status == EBeybladeStatus::Ready) ? false : true;
    }
    // else // AI����̏ꍇ�̏��� ��AI�R���g���[���������邽�߂ɃR�����g�A�E�g
    // {
    //     // AI�̃V���[�g�p���[��ݒ�
    //     LaunchPower = 500.0f;

    //     // AI�̏Ə��x�N�g����ݒ�
    //     AimVector = FVector(-1.7f, 0.0f, 0.0f);

    //     // �v���C���[0�̃|�[�����V���[�g�ς݂��m�F���A���g�̃V���[�g�ς݃t���O�ɔ��f
    //     if (APlayerController *PlayerController0 = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    //     {
    //         if (APawn *ControlledPawn0 = PlayerController0->GetPawn())
    //         {
    //             bIsShot = Cast<ABeybladeBase>(ControlledPawn0)->IsShot();
    //         }
    //     }
    // }

    // �V���[�g��̋��ʏ���
    if (bIsShot)
    {
        // // �\���O�����폜�@��AI�R���g���[���������邽�߂ɃR�����g�A�E�g
        // DeletePrediction();

        // // AI������������ԂȂ�V���[�g�����s
        // if (ControllerType == EBeybladeControllerType::AI)
        // {
        //     if (Status == EBeybladeStatus::Ready)
        //     {
        //         LaunchTriggered();
        //     }
        // }

        // Niagara�G�t�F�N�g����A�N�e�B�u�Ȃ�A�N�e�B�u�ɂ���
        if (NiagaraComponent && !NiagaraComponent->IsActive())
        {
            NiagaraComponent->Activate(false);
        }

        // HP��0�ȉ��Ȃ��~����
        if (HP <= 0.0)
        {
            Stop();
        }

        if (Status != EBeybladeStatus::Stopped)
        {
            // �u�[�X�g���Ȃ�g���C���̐F��Ԃ�
            if (NiagaraComponent && IsBoost)
            {
                NiagaraComponent->SetNiagaraVariableLinearColor(TEXT("TrailColor"), FLinearColor::Red);
            }
            else // ��u�[�X�g���̓u�[�X�g�Q�[�W���񕜂��A�g���C���̐F�𔒂�
            {
                Boost = FMath::Clamp(Boost + BoostRegen * DeltaTime, 0.0f, BoostMax);
                NiagaraComponent->SetNiagaraVariableLinearColor(TEXT("TrailColor"), FLinearColor::White);
            }
        }
    }

    // ����������Ԃ̏ꍇ
    if (Status == EBeybladeStatus::Ready)
    {
        // �\���O�����X�V
        UpdatePrediction();

        // �ҋ@�ʒu�Ƀx�C�u���[�h���Œ�
        if (BeybladeCapsuleComponent)
        {
            BeybladeCapsuleComponent->SetWorldLocation(StandbyPos, false, nullptr, ETeleportType::None);
        }
    }

    // ��~���������ł��Ȃ��ꍇ�i���퓬���j
    else if ((Status != EBeybladeStatus::Stopped) && (Status != EBeybladeStatus::Ready))
    {
        // �X�^�e�B�b�N���b�V���̈ʒu�Ɖ�]���X�V
        if (StaticMeshComponent)
        {
            // �J�v�Z���R���|�[�l���g�̈ʒu�ɍ��킹�ă��b�V���̈ʒu�𒲐�
            FVector NewLocation = BeybladeCapsuleComponent->GetComponentLocation() + FVector(0.0f, 0.0f, -6.5f);

            // ���݂̉�]���擾
            FRotator CurrentRotation = StaticMeshComponent->GetComponentRotation();
            // ��]���x�����Z���ĐV����Yaw���v�Z
            float newRoll = FMath::Clamp(GetVelocity().X * 0.3f, -45.0f, 45.0f);
            float newPitch = FMath::Clamp(GetVelocity().Y * 0.3f, -45.0f, 45.0f);
            float NewYaw = CurrentRotation.Yaw + RotationSpeed * DeltaTime;
            // �V�����ʒu�Ɖ�]��ݒ�
            MeshPitchAndRollComponent->SetWorldLocationAndRotation(NewLocation, FRotator(newPitch, 0, newRoll), false, nullptr, ETeleportType::None);
            StaticMeshComponent->SetWorldRotation(FRotator(0.0f, NewYaw, 0.0f), false, nullptr, ETeleportType::None);
        }

        // �_���[�W�N�[���_�E���^�C�}�[���X�V
        if (DamageCDTimer > 0.0)
        {
            DamageCDTimer = FMath::Clamp(DamageCDTimer - DeltaTime, 0, DamageCD);
        }

        // ���肪����ꍇ�AHP�����R����������
        if (Status != EBeybladeStatus::Ready && bbGameInstance->BeybladeList.Num() > 1)
        {
            HP -= DurabilityCoefficient * DeltaTime;
        }
    }

    // �K�E�Z�����x�����̏���
    if (UltimateCastingDelay > 0.0f)
    {
        UltimateCastingDelay = FMath::Clamp((UltimateCastingDelay - DeltaTime), 0.0f, 5.0f);
        // �x�����I�������K�E�Z�𔭓�
        if (UltimateCastingDelay <= 0.0f)
        {
            bbGameInstance->EnvironmentManager->ToggleUltraLightEffect(false);
            Status = EBeybladeStatus::Ultra;
            IsUltimateUsed = true;
            ActiveUltimate();
        }
    }

    // �K�E�Z���̏���
    if (Status == EBeybladeStatus::Ultra)
    {
        UltimateTick(DeltaTime);
    }
}

// ���͋@�\�Ə������o�C���h���܂�
void ABeybladeBase::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // ���̓A�N�V�����A�Z�b�g�����[�h
    BoostInput = LoadObject<UInputAction>(NULL, TEXT("/Game/IA_Boost"), NULL, LOAD_None, NULL);
    CameraInput = LoadObject<UInputAction>(NULL, TEXT("/Game/IA_Camera"), NULL, LOAD_None, NULL);
    LaunchInput = LoadObject<UInputAction>(NULL, TEXT("/Game/IA_Launch"), NULL, LOAD_None, NULL);
    MoveInput = LoadObject<UInputAction>(NULL, TEXT("/Game/IA_Move"), NULL, LOAD_None, NULL);
    UltraInput = LoadObject<UInputAction>(NULL, TEXT("/Game/IA_Ultra"), NULL, LOAD_None, NULL);
    PauseInput = LoadObject<UInputAction>(NULL, TEXT("/Game/IA_Pause"), NULL, LOAD_None, NULL);

    UEnhancedInputComponent *Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    // ETriggerEvent �̒l��ς��邱�ƂŁA�C�ӂ̃g���K�[�C�x���g�Ƀo�C���h�ł��܂�

    // �e���̓A�N�V�����ƑΉ�����֐����o�C���h
    Input->BindAction(BoostInput, ETriggerEvent::Triggered, this, &ABeybladeBase::BoostTriggered);
    Input->BindAction(BoostInput, ETriggerEvent::Completed, this, &ABeybladeBase::BoostCompleted);
    Input->BindAction(UltraInput, ETriggerEvent::Triggered, this, &ABeybladeBase::UltraTriggered);
    Input->BindAction(LaunchInput, ETriggerEvent::Triggered, this, &ABeybladeBase::LaunchTriggered);
    Input->BindAction(CameraInput, ETriggerEvent::Triggered, this, &ABeybladeBase::CameraTriggered);
    Input->BindAction(MoveInput, ETriggerEvent::Triggered, this, &ABeybladeBase::MoveTriggered);
    Input->BindAction(PauseInput, ETriggerEvent::Triggered, this, &ABeybladeBase::PauseTriggered);
}

// �u�[�X�g���͂��g���K�[���ꂽ���̏���
void ABeybladeBase::BoostTriggered()
{
    // �퓬���łȂ��ꍇ�͏������Ȃ�
    if (Status != EBeybladeStatus::Stopped && Status != EBeybladeStatus::Ready)
    {
        bool canUseBoost = false;
        // �u�[�X�g�����u�[�X�g�Q�[�W���c���Ă���ꍇ
        if (IsBoost)
        {
            if (Boost > 0.0f)
            {
                canUseBoost = true;
            }
        }
        else // ��u�[�X�g���ŁA�u�[�X�g�Q�[�W���Œ�l�ȏ゠��ꍇ
        {
            if (GetBoostPercentage() > BoostCost * InternalDeltaTime)
            {
                canUseBoost = true;
            }
        }

        // �u�[�X�g���g�p�\�Ȃ�A�Q�[�W������ău�[�X�g�t���O�𗧂Ă�
        if (canUseBoost)
        {
            Boost = FMath::Clamp((Boost - (BoostCost * InternalDeltaTime)), 0.0, 100.0);
            IsBoost = true;
        }
        else // �g�p�s�Ȃ�u�[�X�g�t���O�����낷
        {
            IsBoost = false;
        }
    }
}

// �ړ����͂��g���K�[���ꂽ���̏���
void ABeybladeBase::MoveTriggered(const FInputActionInstance &Instance)
{
    // �퓬���łȂ��ꍇ�͏������Ȃ�
    if ((Status == EBeybladeStatus::Stopped) || (Status == EBeybladeStatus::Ready))
    {
        return;
    }

    // �_���[�W�N�[���_�E�����͏������Ȃ�
    if (DamageCDTimer > 0.0f)
    {
        return;
    }

    // �g���N�l���v�Z
    FVector2D ActionValue = Instance.GetValue().Get<FVector2D>();
    FVector2D NormalizedActionValue = UKismetMathLibrary::NormalSafe2D(ActionValue);

    // �ړ��l���[���łȂ��ꍇ�AMove�֐����Ăяo��
    if (!NormalizedActionValue.IsNearlyZero())
    {
        Move(NormalizedActionValue);
    }
}

void ABeybladeBase::Move(const FVector2D &MoveValue)
{
    // �u�[�X�g��Ԃɉ����đ��x�W����I��
    float SpeedMultiplier = IsBoost ? 1.5 : 1.0;

    // �g���N���v�Z
    float TorqueX = MoveValue.X * MoveSpeed * SpeedMultiplier * InternalDeltaTime;
    float TorqueY = MoveValue.Y * MoveSpeed * SpeedMultiplier * InternalDeltaTime;

    // CapsuleComponent�Ƀg���N�i���W�A���P�ʁj��ǉ�
    BeybladeCapsuleComponent->AddTorqueInRadians(FVector(TorqueX, TorqueY, 0.0), NAME_None, false);
}

// �u�[�X�g���͂������������̏���
void ABeybladeBase::BoostCompleted()
{
    IsBoost = false;
}

// �V���[�g���͂��g���K�[���ꂽ���̏���
void ABeybladeBase::LaunchTriggered()
{
    // ����������ԂłȂ��ꍇ�͏������Ȃ�
    if (Status != EBeybladeStatus::Ready)
    {
        return;
    }

    // �X�e�[�^�X���u�ʏ�v�ɕύX
    Status = EBeybladeStatus::Normal;

    if (BeybladeCapsuleComponent)
    {
        // �R���W�����ƕ����V�~�����[�V������L����
        BeybladeCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        BeybladeCapsuleComponent->SetSimulatePhysics(true);

        // �Ə������Ƀp���[���悹�ė͂�������
        FVector Force = (AimVector * LaunchPower);
        BeybladeCapsuleComponent->AddForce(Force, NAME_None, true);
    }

    // �\���O�����폜
    DeletePrediction();

    // �X�v���C���R���|�[�l���g��j��
    if (SplineComponent)
    {
        SplineComponent->DestroyComponent(false);
    }

    // Niagara�G�t�F�N�g��L����
    if (NiagaraComponent)
    {
        NiagaraComponent->SetActive(true);
    }
}

// �|�[�Y���͂��g���K�[���ꂽ���̏���
void ABeybladeBase::PauseTriggered()
{
    // �Q�[�����[�h���擾
    ABeybladeGameModeBase *GameMode = Cast<ABeybladeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

    // �Q�[�����I�����Ă��Ȃ���΃|�[�Y��؂�ւ���
    if (GameMode->GetGameState() != EGameState::Finished)
    {
        GameMode->TogglePause(int(ControllerType));
    }
    else // �Q�[�����I�����Ă���΃��x���������[�h
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), false);
    }
}

// �J����(�Ə�)���͂��g���K�[���ꂽ���̏���
void ABeybladeBase::CameraTriggered(const FInputActionInstance &Instance)
{
    // ���͒l���擾
    float ActionValueX = Instance.GetValue().Get<FVector2D>().X;
    float ActionValueY = Instance.GetValue().Get<FVector2D>().Y;

    // �Ə��x�N�g�����v�Z�i�p�x������K�p�j
    float AimVectorX = FMath::Clamp(AimVector.X + ActionValueY * AimSpeed, ShootAngleMin, ShootAngleMax);
    float AimVectorY = FMath::Clamp(AimVector.Y + ActionValueX * AimSpeed, -0.7, 0.7);

    // �Ə��x�N�g�����X�V
    AimVector = FVector(AimVectorX, AimVectorY, 0.0f);
}

// 2�̃x�N�g���Ԃ̊p�x���v�Z���܂�
float ABeybladeBase::FindAngleBetweenTwoVectors(FVector VecA, FVector VecB)
{
    // �x�N�g���𐳋K��
    FVector NormalizedA = VecA.GetSafeNormal(0.0001f);
    FVector NormalizedB = VecB.GetSafeNormal(0.0001f);
    // ���ς��v�Z
    float DotProduct = FVector::DotProduct(NormalizedA, NormalizedB);
    // ���ς���p�x�i�x���@�j���v�Z���ĕԂ�
    return FMath::RadiansToDegrees(FMath::Acos(DotProduct));
}

// �\���O�����X�V���܂�
void ABeybladeBase::UpdatePrediction()
{
    // �����̗\���O�����폜
    DeletePrediction();

    if (Controller)
    {
        // �\���p�����[�^��ݒ�
        FPredictProjectilePathParams PredictParams;
        PredictParams.StartLocation = BeybladeCapsuleComponent->GetComponentLocation();
        PredictParams.LaunchVelocity = AimVector * 200.0f; // �\���p�̑��x�𒲐�
        PredictParams.bTraceWithCollision = true;
        PredictParams.ProjectileRadius = 6.8f;
        PredictParams.MaxSimTime = 1.0f;
        PredictParams.bTraceWithChannel = true;
        PredictParams.TraceChannel = ECC_WorldStatic;
        PredictParams.ActorsToIgnore.Add(this);
        PredictParams.SimFrequency = 20.0f;
        PredictParams.OverrideGravityZ = 0.0f; // �d�͂̉e���𖳎�
        PredictParams.DrawDebugType = EDrawDebugTrace::None;
        PredictParams.DrawDebugTime = 1.0f;
        PredictParams.bTraceComplex = false;

        // �e���\�������s
        FPredictProjectilePathResult PredictResult;
        if (UGameplayStatics::Blueprint_PredictProjectilePath_Advanced(this, PredictParams, PredictResult))
        {
            // �\�����ʂ̃p�X�f�[�^���X�v���C���̓_�Ƃ��Đݒ�
            TArray<FVector> SplinePoints;
            for (const FPredictProjectilePathPointData &PathData : PredictResult.PathData)
            {
                SplinePoints.Add(PathData.Location);
            }

            SplineComponent->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::World, true);

            // �X�v���C���̊e�Z�O�����g�ɉ����ă��b�V����z�u
            int32 LastIndex = SplineComponent->GetNumberOfSplineSegments() - 1;
            for (int32 Index = 0; Index <= LastIndex; ++Index)
            {
                // �Z�O�����g�̊J�n�ʒu/�^���W�F���g�ƏI���ʒu/�^���W�F���g���擾
                FVector StartPos, StartTangent, EndPos, EndTangent;
                SplineComponent->GetLocationAndTangentAtSplinePoint(Index, StartPos, StartTangent, ESplineCoordinateSpace::World);
                SplineComponent->GetLocationAndTangentAtSplinePoint(Index + 1, EndPos, EndTangent, ESplineCoordinateSpace::World);

                // �X�v���C�����b�V���R���|�[�l���g��V�K�쐬
                FString SegmentName = "SplineMesh" + FString::SanitizeFloat(Index);
                USplineMeshComponent *SplineMeshComp = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass(), *SegmentName);
                SplineMeshComp->RegisterComponent();
                SplineMeshComp->SetMobility(EComponentMobility::Movable);

                // �X�v���C�����b�V���̃v���p�e�B��ݒ�
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

                // �X�v���C�����b�V���̊J�n�ƏI����ݒ�
                SplineMeshComp->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
            }
        }
    }
}

// �\���O���i�X�v���C�����b�V���j���폜���܂�
void ABeybladeBase::DeletePrediction()
{
    // �z����̂��ׂẴX�v���C�����b�V���R���|�[�l���g��j��
    for (USplineMeshComponent *Element : SplineMeshComponentArray)
    {
        if (Element)
        {
            Element->DestroyComponent();
        }
    }
    // �z�����ɂ���
    SplineMeshComponentArray.Empty();
}

// �s�k�҂��Q�[���C���X�^���X�̃��X�g����폜���܂�
void ABeybladeBase::RemoveLoser()
{
    if (bbGameInstance)
    {
        TMap<int32, ABeybladeBase *> &BeybladeList = bbGameInstance->BeybladeList;

        // ���X�g�����g����������폜
        if (BeybladeList.Contains(BeybladeIndex) && *BeybladeList.Find(BeybladeIndex) == this)
        {
            BeybladeList.Remove(BeybladeIndex);
        }
    }
}

// �K�E�Z�������̏����i���N���X�ł͋�A�h���N���X�ŃI�[�o�[���C�h�j
void ABeybladeBase::ActiveUltimate()
{
}

// �_���[�W��^�������̏����i���N���X�ł͋�A�h���N���X�ŃI�[�o�[���C�h�j
void ABeybladeBase::OnDamageDealt()
{
}

// �K�E�Z����Tick����
void ABeybladeBase::UltimateTick(float Deltatime)
{
    // �K�E�Z���łȂ���Ώ������Ȃ�
    if (Status != EBeybladeStatus::Ultra)
    {
        return;
    }

    // �K�E�Z�^�C�}�[�����Z
    if (UltimateTimer > 0.0)
    {
        UltimateTimer -= Deltatime;
    }
    else // �^�C�}�[��0�ɂȂ�����ʏ��Ԃɖ߂�
    {
        UltimateTimer = 0.0;
        Status = EBeybladeStatus::Normal;
    }
}

// �x�C�u���[�h���~�����鏈��
void ABeybladeBase::Stop()
{
    Status = EBeybladeStatus::Stopped;

    // ���̏�������x�������s����Ȃ��悤��
    if (!bHasStopExecuted)
    {
        bHasStopExecuted = true;

        // �s�k�҃��X�g���玩�g���폜
        RemoveLoser();

        if (StaticMeshComponent)
        {
            // �e����f�^�b�`���A�����V�~�����[�V������L�������ĉ�]������
            StaticMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
            StaticMeshComponent->SetSimulatePhysics(true);
            StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
            StaticMeshComponent->AddTorqueInRadians(FVector(0.0f, 0.0f, 500.0f), NAME_None, true);
        }

        if (BeybladeCapsuleComponent)
        {
            // �R���W�����𖳌���
            BeybladeCapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
            BeybladeCapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
        }

        // AxisLocker�őS���̈ړ��Ɖ�]�����b�N
        if (AxisLocker)
        {
            AxisLocker->SetNewConstraintMode(true, true, true, true, true, true);
        }

        // Niagara�G�t�F�N�g��j��
        if (NiagaraComponent)
        {
            NiagaraComponent->DestroyComponent();
        }
    }
}

// �K�E�Z���͂��g���K�[���ꂽ���̏���
void ABeybladeBase::UltraTriggered()
{
    // �^�C���f�B���[�V������1�łȂ��i�X���[���[�V�������Ȃǁj�܂��͕K�E�Z���g�p�s�̏ꍇ�͏������Ȃ�
    if (UGameplayStatics::GetGlobalTimeDilation(GetWorld()) != 1.0f || !CanUseUltimate())
    {
        return;
    }

    // �Q�[���C���X�^���X�ɕK�E�Z�g�p��ʒm
    bbGameInstance->NotifyUltraStatus(this);

    // �^�C���f�B���[�V�������l�����ĕK�E�Z�����܂ł̒x�����Ԃ�ݒ�
    UltimateCastingDelay = 3.0f / (1.0f / UGameplayStatics::GetGlobalTimeDilation(GetWorld()));
}

// �R���|�[�l���g�������Ƀq�b�g�������̏���
void ABeybladeBase::OnComponentHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    // �q�b�g�������肪�X�^�e�B�b�N���b�V���Ȃ珈�����Ȃ�
    if (Cast<UStaticMeshComponent>(OtherComp))
    {
        return;
    }

    // ���g����~��ԂȂ珈�����Ȃ�
    if (Status == EBeybladeStatus::Stopped)
    {
        return;
    }

    // �q�b�g�������肪���̃x�C�u���[�h�̏ꍇ
    if (ABeybladeBase *otherBB = Cast<ABeybladeBase>(OtherActor))
    {
        // ���g�̑��x���擾
        FVector Velocity = BeybladeCapsuleComponent->GetComponentVelocity();

        // ���x�x�N�g���ƏՓ˖@���̊Ԃ̊p�x���v�Z
        float Angle = FindAngleBetweenTwoVectors(Velocity, Hit.ImpactNormal);

        // ���x�̑傫�����擾
        float power = Velocity.Size();

        // �p�x�ƃp���[����_���[�W���v�Z�i�ő�_���[�W�ŃN�����v�j
        float Damage = FMath::Clamp(Angle * power * DamageCoefficient, 0.0f, MaxDamage);

        // ����Ƀ_���[�W��K�p
        UGameplayStatics::ApplyDamage(otherBB, Damage, Controller, this, UDamageType::StaticClass());

        // �Փˈʒu���痣�������̃x�N�g�����v�Z
        FVector ForceDirection = (BeybladeCapsuleComponent->GetComponentLocation() - Hit.Location).GetSafeNormal(0.0001f, FVector::ZeroVector);

        // �Փ˂̏Ռ��͂Ɋ�Â��Ĕ����͂��v�Z
        float ForceMultiplier = FMath::Clamp(
            NormalImpulse.Size() * 100.0f /*�Ռ���*/
                * (UKismetMathLibrary::RandomFloat() * 4.0f + 1.0f) /*�����_���ȌW��*/,
            1000.0f /*�ŏ��l*/, 7000.0f /*�ő�l*/);

        // �����̓x�N�g�����v�Z�i����������̗͂�������j
        FVector Force = ForceDirection * ForceMultiplier + FVector(0.0f, 0.0f, 100.0f);

        // �q�b�g�G�t�F�N�g�𐶐�
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitEffectSystemClass, Hit.ImpactPoint, FRotator::ZeroRotator);

        // ���g�ɔ����͂�������
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

    // ���͒l���[���łȂ��ꍇ�AMove�֐����Ăяo��
    if (!MoveDirection.IsNearlyZero())
    {
        // Move�֐����Ăяo���Ĉړ��������s��
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
        // BoostTriggered �̃��W�b�N���Q�l�Ƀu�[�X�g�\���`�F�b�N
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