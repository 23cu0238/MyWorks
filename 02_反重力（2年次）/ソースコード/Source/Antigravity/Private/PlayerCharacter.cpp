// ------------------------------------------------
// PlayerCharacter.cpp
// �v���[���[�L�����N�^�[�𐧌䂷��N���X
// �쐬�ҁF���N�F�A���ᩗ�
// �쐬�����F2024-11-18
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

//�R���X�g���N�^
APlayerCharacter::APlayerCharacter(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UGravityCharacterMovementComp>(TEXT("UGravityCharacterMovementComp")))
{
	// ���t���[��Tick()���Ăяo���悤�ɐݒ肵�܂�
	PrimaryActorTick.bCanEverTick = true;

	// �J�����u�[�������i�Փ˂��������ꍇ�A�v���[���[�̕��Ɉ����񂹂���j
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;		// �J�����͂��̋����ŃL�����N�^�[�̔w���ǂ�
	CameraBoom->bUsePawnControlRotation = true; // �R���g���[���[�Ɋ�Â��ăA�[������]������

	// ���z���J�����u�[�������i�Փ˂��������ꍇ�A�v���[���[�̕��Ɉ����񂹂���j
	FirstPersonCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("FirstPersonCameraBoom"));
	FirstPersonCameraBoom->SetupAttachment(ACharacter::GetMesh());
	FirstPersonCameraBoom->bUsePawnControlRotation = true; // �R���g���[���[�Ɋ�Â��ăA�[������]������

	//  ���z���J���������
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // �J�������u�[���̐�[�Ɏ��t���A�R���g���[���[�̌����ɍ��킹�ău�[���𒲐�����
	FollowCamera->bUsePawnControlRotation = true;								// �J�������A�[���ɑ΂��ĉ�]���Ȃ�
	ActiveCamera = FollowCamera;

	// �J���������
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(FirstPersonCameraBoom); // �J�������u�[���̐�[�Ɏ��t���A�R���g���[���[�̌����ɍ��킹�ău�[���𒲐�����
	FirstPersonCamera->bUsePawnControlRotation = true;		   // �J�������A�[���ɑ΂��ĉ�]���Ȃ�
	FirstPersonCamera->SetActive(false);

	// �R���W�����J�v�Z���̃T�C�Y��ݒ�
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// �A�C�e���ƃC���^���N�V�����͈͂��쐬
	mInteractComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRange"));
	mInteractComponent->SetupAttachment(GetCapsuleComponent());

	// �A�^�b�N�͈̔͂��쐬
	mAttackComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackComp"));
	mAttackComponent->SetupAttachment(GetCapsuleComponent());

	// �R���g���[���[����]���Ă���]�����Ȃ�
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// �L�����N�^�[�̓�����ݒ肷��
	GetCharacterMovement()->bOrientRotationToMovement = true;			 // ���͂��ꂽ�����ɃL�����N�^�[������
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 500.0f); // ���̉�]����

	// �K�v�Ȓl���`
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// �d�̓Q�[�W�̍ő�l��ݒ�
	mGravityGage = mMaxGage = 100;

	// �����f�[�^���擾
	static ConstructorHelpers::FObjectFinder<USoundCue> HitSoundCueTemp(
		TEXT("/Script/Engine.SoundCue'/Game/SE/HitCue.HitCue'"));

	if (HitSoundCueTemp.Succeeded())
	{
		HitSoundCue = HitSoundCueTemp.Object;
	}

	// �����R���|�[�l���g���쐬
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComponent->bAutoActivate = false;
	// ���[�g�ɃA�^�b�`
	AudioComponent->SetupAttachment(RootComponent);

	// �^�C�����C�����쐬
	FirstPersonTimeline = new FTimeline{};
	ThirdPersonTimeline = new FTimeline{};

	// �J�[�u���擾
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
			// �^�C�����C���X�V���ɌĂяo����郁�\�b�h�̒�`
			FOnTimelineFloat TimelineStepFunc;
			TimelineStepFunc.BindUFunction(this, TEXT("FirstPersonTimelineStep"));
			FirstPersonTimeline->AddInterpFloat(TimelineCurve, TimelineStepFunc);

			// �^�C�����C���I�����ɌĂяo����郁�\�b�h�̒�`
			FOnTimelineEvent TimelineFinishedFunc;
			TimelineFinishedFunc.BindUFunction(this, TEXT("FirstPersonTimelineFinished"));
			FirstPersonTimeline->SetTimelineFinishedFunc(TimelineFinishedFunc);
		}

		if (ThirdPersonTimeline != nullptr)
		{
			// �^�C�����C���X�V���ɌĂяo����郁�\�b�h�̒�`
			FOnTimelineFloat TimelineStepFunc;
			TimelineStepFunc.BindUFunction(this, TEXT("ThirdPersonTimelineStep"));
			ThirdPersonTimeline->AddInterpFloat(TimelineCurve, TimelineStepFunc);

			// �^�C�����C���I�����ɌĂяo����郁�\�b�h�̒�`
			FOnTimelineEvent TimelineFinishedFunc;
			TimelineFinishedFunc.BindUFunction(this, TEXT("ThirdPersonTimelineFinished"));
			ThirdPersonTimeline->SetTimelineFinishedFunc(TimelineFinishedFunc);
		}
	}
}

// �f�X�g���N�^
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

// �X�^�[�g�֐�
void APlayerCharacter::BeginPlay()
{
	// �e�N���X��BeginPlay
	Super::BeginPlay();

	// HP�̏����l��ݒ�
	mHitPoint = 3;

	// �����d�̓X�P�[�����L�^
	mDefaultGravityScale = GetCharacterMovement()->GravityScale;
	mDefaultGravityDirection = GetGravityDirection();

	// �R���V������OnHit�֐��̃o�C���h
	UCapsuleComponent *CollisionComp = GetCapsuleComponent();
	CollisionComp->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnHit);

	// �A�^�b�N�͈͂ƃA�^�b�N�֐����o�C���h
	mAttackComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnAttackHit);

	// ����}�b�s���O��ǉ�����
	if (APlayerController *PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// �e�J�����̏����ʒu���L�^
	FirstPersonCameraOriginalPosition = FirstPersonCamera->GetRelativeTransform();
	ThirdPersonCameraOriginalPosition = FollowCamera->GetRelativeTransform();

	// ������ݒ�
	if (HitSoundCue->IsValidLowLevelFast())
	{
		AudioComponent->SetSound(HitSoundCue);
	}

	// ���X�|���X�ʒu��ݒ�
	UMyGameInstance *instance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (instance)
		SetActorLocation(instance->GetInstance()->GetRespawnPosition());
}

// ���t���[���X�V
void APlayerCharacter::Update()
{
	// ���S����
	if (Dead())
	{
		mPlayerStatus = EPlayerStatus::Died;
	}

	// �d�͕ύX���Ă��Ȃ����ƒn�ʂɈړ����Ă��鎞�J�����̃��[�e�[�V�������O�𖳂���
	if (bNotChangingGravity && GetCharacterMovement()->IsMovingOnGround())
	{
		CameraBoom->bEnableCameraRotationLag = false;
	}

	// �n�ʈړ����Ă��鎞
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		// �����x�A�d�̓X�P�[�������Z�b�g
		mAcceleration = 1;
		GetCharacterMovement()->GravityScale = mDefaultGravityScale;

		// FOV�i����p�j�����Z�b�g
		if (FirstPersonCamera->FieldOfView != 90.0f)
		{
			FirstPersonCamera->FieldOfView = FMath::FInterpTo(FirstPersonCamera->FieldOfView, 90.0f, GetWorld()->GetTime().GetDeltaRealTimeSeconds(), 5.0f);
		}

		// �v���[���[�̃J�����̃s�b�`�����b�N
		Cast<APlayerController>(Controller)->PlayerCameraManager->ViewPitchMax = 50.0f;
	}
	else
	{
		// �d�͉����x�𑝂₷
		GetCharacterMovement()->GravityScale += 0.1;

		// �v���[���[�̃J�����̃s�b�`���A�����b�N�i�قڂX�O�x�ɂ���j
		Cast<APlayerController>(Controller)->PlayerCameraManager->ViewPitchMax = 89.900002f;
	}

	// �d�͕ύX���ł����
	if (mPlayerStatus == EPlayerStatus::ChangingDirection)
	{
		// �W�����v������A�����J�n������
		FVector LocalVelocity = UKismetMathLibrary::LessLess_VectorRotator(GetVelocity(), GetActorRotation());
		if (LocalVelocity.Z < -0.001)
		{
			// �d�͕ύX���[�h�ɓ����Ă鎞
			if (bNotChangingGravity)
			{
				// �J�����̕ύX�^�C�����C�����J�n
				FirstPersonTimeline->PlayFromStart();
				// �d�͕ύX���[�h�ɓ������t���O
				bNotChangingGravity = false;
				// �J�����̃��O�𖳌��ɂ���
				CameraBoom->bEnableCameraRotationLag = false;
			}

			// �J�����̕ύX���I�������
			if (!bIsCameraTransitioning)
			{
				// �v���C���[���g�̂𖳏d�͏�Ԃɂ���
				GetCharacterMovement()->GravityScale = 0.0;
				GetCharacterMovement()->Velocity = FVector::ZeroVector;

				// ���Ԃ̗����x������
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), FMath::FInterpTo(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), mSlowMotionTimeScale, GetWorld()->GetTime().GetDeltaRealTimeSeconds(), 10.0f));
			}
		}

		// �d�͕ύX�X�e�[�^�X�ɓ��������ǁA�ύX���[�h����������Ă��Ȃ���
		if (GetCharacterMovement()->IsMovingOnGround() && !bIsCameraTransitioning && bNotChangingGravity)
		{
			mPlayerStatus = EPlayerStatus::Normal;
		}
	}

	// �d�͕ύX���łȂ����
	else
	{
		// ���Ԃ̗��ꑬ�x�����ɖ߂�
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), FMath::FInterpTo(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), 1.0f, GetWorld()->GetTime().GetDeltaRealTimeSeconds(), 10.0f));
	}

	// ��ʂ̏��
	if (mPlayerStatus == EPlayerStatus::Normal)
	{
		// �N�C�b�N�d�͕ύX�\���ǂ����̃`�F�b�N
		bIsAbleToChangeGravityQuick = CheckIsAbleToChangeGravityQuick();

		// ����FirstPersonCamera���L�����A�d�͕ύX�����ꍇ
		if (FirstPersonCamera->IsActive() && !bNotChangingGravity)
		{
			// �J�����̐؂�ւ�
			ThirdPersonTimeline->PlayFromStart();
			// �d�͕ύX���Ă��Ȃ��t���O�𗧂�
			bNotChangingGravity = true;
		}

		// �J�X�^���d�͂��Ȃ����A�d�̓Q�[�W���P�O�O���ȉ��̏ꍇ�A�P�O�O���܂ő��₷
		if (!GetCharacterMovement()->HasCustomGravity() && mGravityGage < 100.0f)
		{
			mGravityGage += 20.0f * GetWorld()->GetDeltaSeconds();
			if (mGravityGage > 100.0f)
			{
				mGravityGage = 100.0f;
			}
		}

		// �J�X�^���d�͂�����ꍇ�A�d�̓Q�[�W���ǂ�ǂ񌸂炷
		else if (GetCharacterMovement()->HasCustomGravity())
		{
			mGravityGage -= 10.0f * GetWorld()->GetDeltaSeconds();
		}

		// �d�̓Q�[�W���Ȃ��Ȃ����ꍇ�͏d�͕��������Z�b�g
		if (mGravityGage <= 0)
		{
			GetCharacterMovement()->SetGravityDirection(FVector::DownVector);
		}
	}

	// �������E�̃f���^�^�C�����擾
	float RealTimeDeltaSeconds = GetWorld()->GetTime().GetDeltaRealTimeSeconds();

	// �J�����̃^�C�����C�����Đ�����Ă���ꍇ�͂��̃^�C�����C�����`�b�N
	if (FirstPersonTimeline->IsPlaying())
	{
		FirstPersonTimeline->TickTimeline(RealTimeDeltaSeconds);
	}
	if (ThirdPersonTimeline->IsPlaying())
	{
		ThirdPersonTimeline->TickTimeline(RealTimeDeltaSeconds);
	}

	// �d�͕ύX��A���n�܂ł̎��Ԃ��v��
	if (bIsGravityChanged)
	{
		mChangedGravityTimer += GetWorld()->GetDeltaSeconds();
	}

	// �ύX�����u�Ԃ̐ڐG�����h�~
	if (mChangedGravityTimer >= 0.02f)
	{
		bIsWaitingToHitGround = true;
	}

	// ���݂̃p���[���x���𔻒�
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

	// �����U�����̏���
	if (mPlayerStatus == EPlayerStatus::Falling)
	{
		// �ύX�����u�Ԃ̐ڐG�����h�~�t���O�𗧂�����
		if (bIsWaitingToHitGround)
		{
			// FOV���X���[�Y�ɍL������
			FirstPersonCamera->FieldOfView = FMath::FInterpTo(FirstPersonCamera->FieldOfView, 135.0f, GetWorld()->GetTime().GetDeltaRealTimeSeconds(), 5.0f);
		}
		// �A�^�b�N�͈͂�L��
		mAttackComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	}
	else
	{
		// �A�^�b�N�͈͂𖳌�
		mAttackComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// �ꎞ��~���Ă��Ȃ���
	if (mPauseMenuWidgetInstance && !mPauseMenuWidgetInstance->IsInViewport())
	{
		Cast<APlayerController>(GetController())->bShowMouseCursor = false;
	}
}

// ���̏d�͕����ɖ߂�֐�
void APlayerCharacter::BackToNormalGravity()
{
	if (GetGravityDirection() != FVector::DownVector)
	{
		// �J������ʏ펞�ɖ߂�
		ThirdPersonTimeline->PlayFromStart();
		FirstPersonCamera->FieldOfView = FMath::FInterpTo(FirstPersonCamera->FieldOfView, 90.0f, GetWorld()->GetTime().GetDeltaRealTimeSeconds(), 5.0f);

		// �d�͕��������Z�b�g
		GetCharacterMovement()->SetGravityDirection(FVector::DownVector);

		// �t���O�ƃX�e�[�^�X�ƃ^�C�}�[�����Z�b�g
		bIsWaitingToHitGround = false;
		bIsGravityChanged = false;
		mPlayerStatus = EPlayerStatus::Normal;
		mChangedGravityTimer = 0.0f;
	}
}

// �ړ�����
void APlayerCharacter::Move(const FInputActionValue &Value)
{
	// ���S���Ȃ�����Ȃ�
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return;
	}

	// �ړ������x�N�g�����擾
	FVector2D MovementVector = Value.Get<FVector2D>();

	// �R���g���[�������݂��鎞
	if (AGravityController *GravityController = Cast<AGravityController>(Controller))
	{
		// �J�����̃��[�e�[�V�������擾
		const FRotator Rotation = Controller->GetControlRotation();

		// �d�͕����ɉ�������]�̕␳
		const FRotator GravityRelativeRotation = GravityController->GetGravityRelativeRotation(Rotation, GetGravityDirection());
		const FRotator GravityWorldRotation = GravityController->GetGravityWorldRotation(GravityRelativeRotation, GetGravityDirection());

		// ���݂̑O�x�N�g���ƉE�x�N�g�����擾
		const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(GravityWorldRotation);
		const FVector RightDirection = UKismetMathLibrary::GetRightVector(GravityWorldRotation);

		// �ړ�������
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

// �d�͕ύX���L�����Z��
void APlayerCharacter::CancelGravity()
{
	// ���S���Ȃ�����Ȃ�
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return;
	}

	// �J�������ύX���Ă��Ȃ��Ƃ�
	if (!bIsCameraTransitioning)
	{
		// �ύX��
		if (mPlayerStatus == EPlayerStatus::ChangingDirection && ActiveCamera == FirstPersonCamera)
		{
			// �X�e�[�^�X�����Z�b�g
			mPlayerStatus = EPlayerStatus::Normal;
		}

		// �����U����
		else if (mPlayerStatus == EPlayerStatus::Falling)
		{
			// �d�͂����Z�b�g
			BackToNormalGravity();
		}
	}
}

// �d�͂����肶�̏���
void APlayerCharacter::ApplyGravity()
{
	// ���S���Ȃ�����Ȃ�
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return;
	}

	// �d�͕ύX���ł͂Ȃ����͉������Ȃ�
	if (mPlayerStatus != EPlayerStatus::ChangingDirection)
	{
		return;
	}

	// �d�͕ύX���ƃJ�����ύX���Ă��Ȃ���
	if (!bNotChangingGravity && !bIsCameraTransitioning)
	{
		// �d�͉����x�����Z�b�g�A�������J�����̐��ʂɂ���
		GetCharacterMovement()->GravityScale = 1.0;
		GetCharacterMovement()->SetGravityDirection(FollowCamera->GetForwardVector());

		// �X�e�[�^�X�𗎉��U�����ɂ���
		mPlayerStatus = EPlayerStatus::Falling;

		// �t���O�𗧂�
		bIsGravityChanged = true;
		bNotChangingGravity = true;

		// �J�����̉�]���O��L����
		CameraBoom->bEnableCameraRotationLag = true;

		// �d�͖ڕW�n�_�̏�������
		bNotHit();
	}
}

// �N�C�b�N�d�͕ύX�\���ǂ������`�F�b�N����֐�
bool APlayerCharacter::CheckIsAbleToChangeGravityQuick()
{
	// ���S���Ȃ�����Ȃ�
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return false;
	}

	// �X�e�[�^�X���ʏ펞����
	if (mPlayerStatus == EPlayerStatus::Normal)
	{
		// ���C�L���X�g���g���āA�J�����̒����ɂ���I�u�W�F�N�g��������n�ʂ��ǂ������`�F�b�N
		// ������ꍇ�͏d�͕ύX�ڕW�n�_���L�^���Atrue��Ԃ�
		// �����Ȃ��ꍇ�d�͕ύX�ڕW�n�_�������Afalse��Ԃ�
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

// �N�C�b�N�d�͕ύX�֐�
void APlayerCharacter::ChangeGravityQuick()
{
	// ���S���Ȃ�����Ȃ�
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return;
	}

	// �X�e�[�^�X���ʏ펞����
	if (mPlayerStatus == EPlayerStatus::Normal)
	{
		// ���C�L���X�g���g���āA�J�����̒����ɂ���I�u�W�F�N�g��������n�ʂł���΁A
		// �d�͕��������̖ʂ̐��������ɂ���
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

// �d�͕ύX�֐�
void APlayerCharacter::ChangeGravity()
{
	// ���S���Ȃ�����Ȃ�
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return;
	}

	// �J�������ύX���ł͂Ȃ�������
	if (!bIsCameraTransitioning)
	{
		// �X�e�[�^�X���ʏ펞����
		if (mPlayerStatus == EPlayerStatus::Normal)
		{
			// ���W�����v���āA�X�e�[�^�X���d�͕ύX���ɂ���
			ACharacter::Jump();
			mPlayerStatus = EPlayerStatus::ChangingDirection;
		}
	}
}

// �q�b�g����
void APlayerCharacter::OnHit(UPrimitiveComponent *HitCom, AActor *OtherActor, UPrimitiveComponent *OtherComp,
							 FVector NormalImpulse, const FHitResult &Hit)
{
	// ���S���Ȃ�����Ȃ�
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return;
	}

	// �X�e�[�^�X���ʏ펞�ł���Ή������Ȃ�
	if (mPlayerStatus == EPlayerStatus::Normal)
	{
		return;
	}

	// �ύX�����u�Ԃ̐ڐG�����h�~�t���O�����Ă鎞�ƐڐG���Ă���I�u�W�F�N�g��������n�ʂł����
	if (bIsWaitingToHitGround && OtherActor->ActorHasTag("Walkable"))
	{
		// ���x���[���ɂ���
		GetCharacterMovement()->Velocity = FVector::ZeroVector;

		// �J������ύX
		ThirdPersonTimeline->PlayFromStart();

		// �d�͕�����ڐG���Ă���n�ʂ̐��������ɂ���
		GetCharacterMovement()->SetGravityDirection(UKismetMathLibrary::GetDirectionUnitVector(Hit.ImpactNormal * 100.0f + Hit.ImpactPoint, Hit.ImpactPoint));

		// �X�e�[�^�X�A�t���O�����Z�b�g
		mPlayerStatus = EPlayerStatus::Normal;
		bIsWaitingToHitGround = false;
		bIsGravityChanged = false;

		// �����𐶐�
		SpawnExplosionDelegate.Broadcast(mPowerLevel);

		// �d�͕ύX�^�C�}�[�����Z�b�g
		mChangedGravityTimer = 0.0f;
	}
}

// �A�^�b�N����
void APlayerCharacter::OnAttackHit(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
								   int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// ���S���Ȃ�����Ȃ�
	if (mPlayerStatus == EPlayerStatus::Died)
	{
		return;
	}

	// �ڐG�����̂̓G�l�~�[�̏ꍇ
	if (OtherActor->ActorHasTag("Enemy"))
	{
		// �������Đ�
		AudioComponent->Play();

		// �����HP�����炷
		TObjectPtr<UConventionalAttack> attackObj = NewObject<UConventionalAttack>();
		attackObj->Attack((float)mPowerLevel, OtherActor);
	}
}

// ���z���J�����ɕύX����^�C�����C���̃`�b�N
void APlayerCharacter::FirstPersonTimelineStep(float _Value)
{
	if (FirstPersonCamera && FollowCamera)
	{
		// �ύX�O�̃J�������X���[�Y�ɐV�J�����̈ʒu�Ɉړ�
		FTransform newTransform = UKismetMathLibrary::TLerp(FollowCamera->GetComponentTransform(), FirstPersonCamera->GetComponentTransform(), _Value);
		FollowCamera->SetWorldTransform(newTransform);

		// �v���[���[�L�����N�^�[���J�����̐��ʂɃX���[�Y�ɉ�]������
		SetActorRotation(FQuat::Slerp(GetActorRotation().Quaternion(), GetControlRotation().Quaternion(), 0.3f));
	}
}

// ��ʃJ�����ɕύX����^�C�����C���̃`�b�N
void APlayerCharacter::ThirdPersonTimelineStep(float _Value)
{
	if (FirstPersonCamera && FollowCamera)
	{
		// �ύX�O�̃J�������X���[�Y�ɐV�J�����̈ʒu�Ɉړ�
		FTransform newTransform = UKismetMathLibrary::TLerp(FirstPersonCamera->GetComponentTransform(), FollowCamera->GetComponentTransform(), _Value);
		FirstPersonCamera->SetWorldTransform(newTransform);
	}
}

// ���z���J�����ɕύX����^�C�����C���Đ���������Ƃ��̏���
void APlayerCharacter::FirstPersonTimelineFinished()
{
	if (FirstPersonCamera && FollowCamera)
	{
		//�ڕW�J������L�������A�s�v�ȃJ�����𖳌���
		FirstPersonCamera->SetActive(true);
		FollowCamera->SetActive(false);
		ActiveCamera = FirstPersonCamera;
		
		//�s�v�ȃJ�����̈ʒu�����̈ʒu�ɖ߂�
		FollowCamera->SetRelativeTransform(ThirdPersonCameraOriginalPosition);

		//���z���J�����̉�]���X�V
		UpdateFPSCameraRotation();

		//�v���C���[�̉�]���J�����Ƒ�����
		bUseControllerRotationPitch = true;
		bUseControllerRotationRoll = true;
		bUseControllerRotationYaw = true;

		//�J�����ړ������t���O�𗧂�
		bIsCameraTransitioning = false;
	}
}

// �ʏ�J�����ɕύX����^�C�����C���Đ���������Ƃ��̏���
void APlayerCharacter::ThirdPersonTimelineFinished()
{
	if (FirstPersonCamera && FollowCamera)
	{
		//�ڕW�J������L�������A�s�v�ȃJ�����𖳌���
		FollowCamera->SetActive(true);
		FirstPersonCamera->SetActive(false);
		ActiveCamera = FollowCamera;

		//�s�v�ȃJ�����̈ʒu�����̈ʒu�ɖ߂�
		FirstPersonCamera->SetRelativeTransform(FirstPersonCameraOriginalPosition);
		
		//�v���C���[�̉�]���J�����Ƒ����Ȃ��悤�ɂ���
		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		bUseControllerRotationYaw = false;

		//�J�����ړ������t���O�𗧂�
		bIsCameraTransitioning = false;
	}
}

// ���z���J�����̉�]���X�V
void APlayerCharacter::UpdateFPSCameraRotation()
{
	//�d�͕�����錾
	FVector GravityDirection = FVector::DownVector;

	if (UCharacterMovementComponent *MoveComp = GetCharacterMovement())
	{
		//�L�����N�^�[���[�u�����g�R���|�[�l���g�����݂���΂��ꂩ�猻�݂̏d�͕������擾����
		GravityDirection = MoveComp->GetGravityDirection();
	}

	// ���[���h��ԂŌ��݂̃R���g���[����]���擾����
	FRotator ViewRotation = GetActorForwardVector().Rotation();

	// �r���[�̉�]�����[���h��Ԃ���d�͑��΋�Ԃɕϊ�����
	// Now we can work with the rotation as if no custom gravity was affecting it.
	ViewRotation = AGravityController::GetGravityRelativeRotation(ViewRotation, GravityDirection);

	// ViewRotation�ɓK�p�����f���^���v�Z����
	FRotator DeltaRot(Cast<APlayerController>(Controller)->RotationInput);

	if (TObjectPtr<APlayerCameraManager> CameraManager = Cast<APlayerController>(Controller)->PlayerCameraManager)
	{
		CameraManager->ProcessViewRotation(GetWorld()->GetTime().GetDeltaRealTimeSeconds(), ViewRotation, DeltaRot);

		// �J�����̃��[�����[���ɂ��A�d�͂ɑ΂��ď�ɐ����ɂ���
		ViewRotation.Roll = 0;

		// ��]�����[���h��Ԃɖ߂��A���݂̃R���g���[����]�Ƃ��Đݒ肷��
		Controller->SetControlRotation(AGravityController::GetGravityWorldRotation(ViewRotation, GravityDirection));

		ViewRotation = GetControlRotation();
	}
}

// ���͊֐����o�C���h
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	// �A�N�V�����o�C���f�B���O�̐ݒ�
	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		// �W�����v
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// �ړ�
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// �J��������
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		// �d�͕ύX
		EnhancedInputComponent->BindAction(ChangeGravityAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ChangeGravity);

		// �d�͕ύX����
		EnhancedInputComponent->BindAction(ApplyGravityAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ApplyGravity);

		// �d�͕ύX�L�����Z��
		EnhancedInputComponent->BindAction(GravityCancelAction, ETriggerEvent::Triggered, this, &APlayerCharacter::CancelGravity);

		// �N�C�b�N�d�͕ύX
		EnhancedInputComponent->BindAction(QuickGravityChangeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ChangeGravityQuick);

		// �`���[�W
		EnhancedInputComponent->BindAction(ChargeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Charge);

		// �ꎞ��~
		EnhancedInputComponent->BindAction(CommandAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Pause);
	}
}

// HP���炷����
void APlayerCharacter::FluctuationHP(float decrease)
{
	//�����U�����͖��G
	if (mPlayerStatus == EPlayerStatus::Falling)
	{
		return;
	}

	//�e�N���X��HP���炷����
	Super::FluctuationHP(decrease);
}

// �J��������
void APlayerCharacter::Look(const FInputActionValue &Value)
{
	//�J�����؂�ւ����ł���Ώ������Ȃ�
	if (bIsCameraTransitioning)
		return;

	//���̒l���擾����
	FVector LookAxisVector = Value.Get<FVector>();

	if (Controller != nullptr)
	{
		// �R���g���[���Ƀ��[�ƃs�b�`�̓��͂�ǉ�����
		AddControllerYawInput(LookAxisVector.X * mCameraSensitivity);
		AddControllerPitchInput(LookAxisVector.Y * mCameraSensitivity);
	}
}

// �ꎞ��~
void APlayerCharacter::Pause()
{
	//�ꎞ��~���j���[�����݂����
	if (mPauseMenuWidget)
	{
		//�ꎞ��~���j���[�̃C���X�^���X�����݂��Ȃ���΍쐬����
		if (!mPauseMenuWidgetInstance)
		{
			mPauseMenuWidgetInstance = CreateWidget(Cast<APlayerController>(Controller), mPauseMenuWidget);
		}

		//���ɕ\�����ł���Δ�\���ɂ��A��\�����ł���Ε\���ɂ���
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

// �`���[�W�i�p�āj
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