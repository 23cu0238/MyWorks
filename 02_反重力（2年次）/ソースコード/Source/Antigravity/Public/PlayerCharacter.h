// ------------------------------------------------
// PlayerCharacter.h
// �v���[���[�L�����N�^�[�𐧌䂷��N���X
// �쐬�ҁF���N�F�A���ᩗ�
// �쐬�����F2024-11-18
// ------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/TimelineComponent.h"
#include "Components/SphereComponent.h"
#include "ActorBase.h"
#include "GravityCharacterMovementComp.h"
#include "Logging/LogMacros.h"
#include "Blueprint/UserWidget.h"
#include "ChangeGravityComponent.h"
#include "PlayerCharacter.generated.h"

// �O���錾
class UInputMappingContext;
class UInputAction;
class APlayerAttackRange;
class USoundCue;
struct FInputActionValue;

// ���O�J�e�S���[�̒�`
DECLARE_LOG_CATEGORY_EXTERN(pLogTemplateCharacter, Log, All);

// �d�͕ύX�ڕW�n�_�̍\����
USTRUCT(BlueprintType)
struct FTargetGravitySpotInfo
{
	GENERATED_USTRUCT_BODY()

	// �R���X�g���N�^
	FTargetGravitySpotInfo()
	{
		bIsHit = false;
		SpotLocation = FVector::ZeroVector;
		SpotNormal = FVector::ZeroVector;
	}

	// �������Ă���̂�
	UPROPERTY(BlueprintReadWrite)
	bool bIsHit;

	// �ڕW�n�_
	UPROPERTY(BlueprintReadWrite)
	FVector SpotLocation;

	// �ڕW�@��
	UPROPERTY(BlueprintReadWrite)
	FVector SpotNormal;
};

// �v���[���[�̏�Ԃ��`����enum
UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	// ���
	Normal = (0x0),
	// �d�͕ύX��
	ChangingDirection = (1 << 0),
	// �U��
	Attacking = (1 << 1),
	// ���n
	Grounding = (1 << 2),
	// �_���[�W��
	Damaged = (1 << 3),
	// ���S
	Died = (1 << 4),
	// ������
	Falling = (1 << 5),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FExplosionDelegate, int, Power);

// �N���X�̐錾
UCLASS()
class ANTIGRAVITY_API APlayerCharacter : public AActorBase
{
	GENERATED_BODY()
public:
	// �R���X�g���N�^
	APlayerCharacter(const FObjectInitializer &ObjectInitializer);
	// �f�X�g���N�^
	virtual ~APlayerCharacter();

public:
	// ������
	void Init() override {};

	// �X�V
	virtual void Update() override;

	// ���̏d�͕����ɖ߂�
	void BackToNormalGravity();

	// �d�͕ύX�֐�
	void ChangeGravity();

	// �N�C�b�N�d�͕ύX�ł��邩�ǂ������`�F�b�N
	bool CheckIsAbleToChangeGravityQuick();

	// �d�͕ύX�֐�
	void ChangeGravityQuick();

	// �d�͕ύX���[�h�ύX
	void ActivateSlowMotion();

	// �@�d�̓Q�[�W���擾�pB
	UFUNCTION(BlueprintCallable)
	float GetGravityGage() const { return mGravityGage; }

	// ���n�n�_�̏��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gravity, meta = (AllowPrivateAccess = "true"))
	struct FTargetGravitySpotInfo GravitySpot;

	// �J�������x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOption, meta = (AllowPrivateAccess = "true"))
	float mCameraSensitivity = 1.0f;

	// �����𐶐��֐��f���P�[�g
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegate")
	FExplosionDelegate SpawnExplosionDelegate;

	// ��Ԃ��擾�p
	UFUNCTION(BlueprintCallable)
	EPlayerStatus GetStatus() const { return mPlayerStatus; }

	// ��Ԃ��擾�p
	UFUNCTION(BlueprintCallable)
	void SetStatus(EPlayerStatus newStatus) { mPlayerStatus = newStatus; }

	// �d�̓Q�[�W�̊������擾�p
	UFUNCTION(BlueprintCallable)
	float GetGravityPercent() const { return mGravityGage / mMaxGage; }

	// HP���擾�p
	UFUNCTION(BlueprintCallable)
	int GetPlayerHP() const { return mHitPoint; }

	// �����U���o�ߎ��Ԃ��擾�p
	UFUNCTION(BlueprintCallable)
	float GetFallingTime() const { return mChangedGravityTimer; }

	// �����U�����x�����擾�p
	UFUNCTION(BlueprintCallable)
	int GetPowerLevel() const { return mPowerLevel; }

	// ���݂̃J�������擾
	UFUNCTION(BlueprintCallable)
	UCameraComponent *GetActiveCamera() const { return ActiveCamera; }

	// �d�̓Q�[�W�̍ő�l���擾
	UFUNCTION(BlueprintCallable)
	float GetMaxGravityGauge() { return mMaxGage; }

	// �d�̓Q�[�W�̍ő�l�𒲐��p�֐�
	UFUNCTION(BlueprintCallable)
	void IncreaseMaxGravityGauge(const float amount)
	{
		mMaxGage += amount;
	}

	// �d�̓Q�[�W�̒l�𒲐��p�֐�
	UFUNCTION(BlueprintCallable)
	void IncreaseGravityGauge(const float amount)
	{
		mGravityGage += amount;
	}

protected:
	// �ړ�
	void Move(const FInputActionValue &Value);

	// �d�͕ύX��������
	void CancelGravity();

	// �d�͕ύX����
	void ApplyGravity();

	// �J��������
	void Look(const FInputActionValue &Value);

	// �ꎞ��~
	void Pause();

	// �ꎞ��~
	void Charge();

	// �r�M���v���C
	virtual void BeginPlay();

	// ���͊֐����o�C���h
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	// HP�����炷�֐�
	virtual void FluctuationHP(float decrease) override;

private:
	// �N�C�b�N�d�͕ύX�\��
	bool bIsAbleToChangeGravityQuick;

	// �G�ɖ������̉���
	USoundCue *HitSoundCue;

	// �����R���|�[�l���g
	UAudioComponent *AudioComponent;

	// �v���C���[�̃X�e�[�^�X
	UPROPERTY()
	EPlayerStatus mPlayerStatus = EPlayerStatus::Normal;

	// �q�b�g����
	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent *HitCom, AActor *OtherActor, UPrimitiveComponent *OtherComp,
			   FVector NormalImpulse, const FHitResult &Hit);

	// �q�b�g����
	UFUNCTION(BlueprintCallable)
	void OnAttackHit(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
					 int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	// �d�͕ύX���ꂽ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gravity, meta = (AllowPrivateAccess = "true"))
	float mChangedGravityTimer = 0;

	// �v���C���[�̃p���[���x�����オ�邽�߂̕b��
	// �C���f�b�N�X�̓��x��
	// �����o�[�ϐ��͂��̎���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<float> mPowerLevelTimeThreshold;

	// �X���[���[�V�����^�C���̃^�C���X�P�[��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float mSlowMotionTimeScale;

	// �f�t�H���g�̑���}�b�s���O
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext *DefaultMappingContext;

	// �W�����v�A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *JumpAction;

	// �ړ��A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *MoveAction;

	// �J��������A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *LookAction;

	// �J�[�\����\������A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *ShowCursor;

	// �d�͕ύX�A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *ChangeGravityAction;

	// �d�͕ύX�A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *QuickGravityChangeAction;

	// �d�̓`���[�W�A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *ChargeAction;

	// �d�̓`���[�W�A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *GravityCancelAction;

	// �d�͕ύX�m��A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *ApplyGravityAction;

	// �ꎞ��~�A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *CommandAction;

	// �ꎞ��~���j���[�E�B�W�F�b�g
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SerializeField, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> mPauseMenuWidget;

	// �ꎞ��~���j���[�E�B�W�F�b�g�̃C���X�^���X
	UPROPERTY()
	UUserWidget *mPauseMenuWidgetInstance;

	// �d�͕ύX�\�ȍő勗��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SerializeField, meta = (AllowPrivateAccess = "true"))
	float mGreatestGravityChangeableDistance;

	// �G�f�B�^���g���Ă��邩�ǂ���(�f�o�b�O�p)
	UFUNCTION(BlueprintPure, Category = Debug)
	bool IsWithEditor() const
	{
#if WITH_EDITOR
		return true;
#else
		return false;
#endif
	}

private:
	// �J�����X�v�����O�A�[��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent *CameraBoom;

	// ���z���J�����X�v�����O�A�[��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent *FirstPersonCameraBoom;

	// �J�����R���|�[�l���g
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent *FollowCamera;

	// ���z���J�����R���|�[�l���g
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent *FirstPersonCamera;

	// ���݂̃J����
	UCameraComponent *ActiveCamera;

	// �d�͕ύX�R���|�[�l���g
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ChangeGravityComp, meta = (AllowPrivateAccess = "true"))
	UChangeGravityComponent *mChangeGravityComponent;

	// �U���͈�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HitComp, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent *mAttackComponent;

	// �A�C�e���ƃC���^���N�V�����͈�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = OverlapComp, meta = (AllowPrivateAccess = "true"))
	USphereComponent *mInteractComponent;

	// �d�͂�ω��ł���X���[�̏��
	bool bNotChangingGravity = true;

	// �J�������؂�ւ������ǂ���
	bool bIsCameraTransitioning = false;

	// �U�������ǂ���
	bool mIsAttack = false;

	// �f�t�H���g�̏d�̓X�P�[��
	float mDefaultGravityScale;

	// �����x
	float mAcceleration = 1;

	// �ő�d�̓Q�[�W
	float mMaxGage;

	// �d�̓Q�[�W
	float mGravityGage;

	// ���݂̃p���[���x��
	int mPowerLevel = 0;

	// �J�����؂�ւ��p�^�C�����C��
	// UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FTimeline *FirstPersonTimeline;

	// UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FTimeline *ThirdPersonTimeline;

	// �J�[�u
	UCurveFloat *TimelineCurve;

	// �J�����̃f�t�H���g�ʒu
	FTransform FirstPersonCameraOriginalPosition;
	FTransform ThirdPersonCameraOriginalPosition;

	// ���z���J�����^�C�����C���X�V���ɌĂ΂�鏈��
	UFUNCTION()
	void FirstPersonTimelineStep(float _Value);

	// ��ʃJ�����^�C�����C���X�V���ɌĂ΂�鏈��
	UFUNCTION()
	void ThirdPersonTimelineStep(float _Value);

	// ���z���J�����^�C�����C���I�����ɌĂ΂�鏈��
	UFUNCTION()
	void FirstPersonTimelineFinished();

	// ��ʃJ�����^�C�����C���I�����ɌĂ΂�鏈��
	UFUNCTION()
	void ThirdPersonTimelineFinished();

	// ���z���J�����̃��[�e�[�V�������X�V
	FORCEINLINE
	void UpdateFPSCameraRotation();

	// �f�t�H���g�d�͕���
	FVector mDefaultGravityDirection;

	// �^�[�Q�b�g�d�͕���
	FVector mTargetGravityDirection;

	// �d�͕ϊ����ꂽ���ǂ���
	bool bIsGravityChanged = false;

	// ���n�҂���Ԓ����ǂ���
	bool bIsWaitingToHitGround = false;

	// ���C�L���X�g�������Ă��Ȃ����̏���
	FORCEINLINE void bNotHit()
	{
		GravitySpot.bIsHit = false;
		mTargetGravityDirection = FVector::Zero();
		GravitySpot.SpotLocation = FVector::Zero();
		GravitySpot.SpotNormal = FVector::Zero();
	}
};
