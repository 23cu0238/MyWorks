// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// ABeybladeBase.h
// �x�C�u���[�h�̊�{�N���X
// �쐬���F2025/03/04

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/Texture2D.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "BeybladeBase.generated.h"

// �x�C�u���[�h�̃X�e�[�^�X���`����񋓑�
UENUM(BlueprintType)
enum class EBeybladeStatus : uint8
{
	Ready = 0 UMETA(DisplayName = "�ҋ@"),
	Normal = 1 UMETA(DisplayName = "�ʏ�"),
	Stopped = 2 UMETA(DisplayName = "��~"),
	Ultra = 3 UMETA(DisplayName = "�K�E"),
};

// �x�C�u���[�h�̃R���g���[���[�^�C�v���`����񋓑�
UENUM(BlueprintType)
enum class EBeybladeControllerType : uint8
{
	None = 0,
	P1 = 1,
	P2 = 2,
	AI = 3,
};

UCLASS()
class BEYBLADE_PROTOTYPE_API ABeybladeBase : public ACharacter
{
	GENERATED_BODY()

	// �֐�
public:
	// ���̃L�����N�^�[�̃v���p�e�B�̃f�t�H���g�l��ݒ肵�܂�
	ABeybladeBase();

protected:
	// �Q�[���J�n���܂��͐������ꂽ���ɌĂ΂�܂�
	virtual void BeginPlay() override;

	// �R���g���[���[�̊��蓖�Ă��m�F���܂�
	void CheckController();

	// �_���[�W��^�����Ƃ��ɌĂяo����܂�
	virtual void OnDamageDealt();

public:
	// ���t���[���Ă΂�܂�
	virtual void Tick(float DeltaTime) override;

	// ���͋@�\���o�C���h���邽�߂ɌĂ΂�܂�
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	// ��]���x���擾���܂�
	UFUNCTION(BlueprintCallable)
	virtual float GetRotationSpeed() const { return RotationSpeed; }

	// �x�C�u���[�h�̈ړ����x���擾���܂�
	UFUNCTION(BlueprintCallable)
	virtual float GetMoveSpeed() const { return MoveSpeed; }

	// �x�C�u���[�h�̌���HP���擾���܂�
	UFUNCTION(BlueprintCallable)
	virtual float GetHP() const { return HP; }

	// �K�E�Z�����Ɏg�p���ꂽ���ǂ������擾���܂�
	UFUNCTION(BlueprintCallable)
	virtual bool GetIsUltimateUsed() const { return IsUltimateUsed; }

	// �x�C�u���[�h�̌��݂̃X�e�[�^�X���擾���܂�
	UFUNCTION(BlueprintCallable)
	virtual EBeybladeStatus GetStatus() const { return Status; }

	// �K�E�Z�̃^�C�g���e�N�X�`�����擾���܂�
	UFUNCTION(BlueprintCallable)
	virtual UTexture2D *GetUltimateTitleTexture() const { return UltimateTitle; }

	// ���̃x�C�u���[�h��HP�������擾���܂�
	UFUNCTION(BlueprintCallable)
	float GetHPPercentage() const { return HP / MaxHP; }

	// ���̃x�C�u���[�h�̃u�[�X�g�Q�[�W�̊������擾���܂�
	UFUNCTION(BlueprintCallable)
	float GetBoostPercentage() const { return Boost / BoostMax; }

	// ���̃x�C�u���[�h�̖��O���擾���܂�
	UFUNCTION(BlueprintCallable)
	FString GetName() const { return BeybladeName; }

	// ���̃x�C�u���[�h�̃R���g���[���[�^�C�v���擾���܂�
	UFUNCTION(BlueprintCallable)
	EBeybladeControllerType GetControllerType() const { return ControllerType; }

	// ���̃x�C�u���[�h���V���[�g���ꂽ���ǂ������擾���܂�
	UFUNCTION(BlueprintCallable)
	bool IsShot() const { return bIsShot; }

	// �K�E�Z���g�p�\���ǂ�����Ԃ��܂�
	UFUNCTION(BlueprintCallable)
	bool CanUseUltimate() const
	{
		return (!(IsUltimateUsed || Status != EBeybladeStatus::Normal || GetHPPercentage() > 0.5f));
	}

protected:
	// �u�[�X�g���͂��g���K�[���ꂽ���̏����ł�
	void BoostTriggered();

	// �ړ����͂��g���K�[���ꂽ���̏����ł�
	void MoveTriggered(const FInputActionInstance &Instance);

    void Move(const FVector2D &MoveValue);

    // �u�[�X�g���͂������������̏����ł�
	void BoostCompleted();

	// �V���[�g���͂��g���K�[���ꂽ���̏����ł�
	void LaunchTriggered();

	// �|�[�Y���͂��g���K�[���ꂽ���̏����ł�
	void PauseTriggered();

	// �J����������͂��g���K�[���ꂽ���̏����ł�
	void CameraTriggered(const FInputActionInstance &Instance);

	// �_���[�W���󂯂����̏����ł�
	UFUNCTION()
	void AnyDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType, class AController *InstigatedBy, AActor *DamageCauser);

	// 2�̃x�N�g���Ԃ̊p�x�����߂܂�
	UFUNCTION(BlueprintCallable)
	float FindAngleBetweenTwoVectors(FVector VecA, FVector VecB);

	// �\�����C�����X�V���܂�
	UFUNCTION(BlueprintCallable)
	void UpdatePrediction();

	// �\�����C�����폜���܂�
	UFUNCTION(BlueprintCallable)
	void DeletePrediction();

	// �s�k�����x�C�u���[�h���Q�[���C���X�^���X����폜���܂�
	UFUNCTION(BlueprintCallable)
	void RemoveLoser();

	// ��~�������Ɉ�x�����Ăяo����܂�
	UFUNCTION(BlueprintCallable)
	void Stop();

	// �K�E�Z���͂��g���K�[���ꂽ���̏����ł�
	UFUNCTION(BlueprintCallable)
	void UltraTriggered();

	// �R���|�[�l���g���q�b�g�������̏����ł�
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

	// �x�C�u���[�h�̈ړ����͂�ǉ����܂�(AI)
	UFUNCTION(BlueprintCallable)
    void AddMoveInput(FVector2D MoveDirection, bool bIsBoosting);

	// �x�C�u���[�h���V���[�g���܂�(AI)
	UFUNCTION(BlueprintCallable)
    void PerformLaunch(FVector AimDirection, float Power);

	// �x�C�u���[�h�̃u�[�X�g�X�e�[�^�X��ݒ肵�܂�(AI)
	UFUNCTION(BlueprintCallable)
	void SetBoostStatus(bool bNewBoostStatus);

protected:
	// �K�E�Z�𔭓�����ۂɈ�x�����Ă΂�܂�
	virtual void ActiveUltimate();

	// �K�E�Z���������̖��t���[���Ă΂�鏈���ł�
	virtual void UltimateTick(float Deltatime);

	// �v���p�e�B
public:
	// ���̃x�C�u���[�h�̃C���f�b�N�X
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	int32 BeybladeIndex = -1;

protected:
	// Niagara�V�X�e���̃N���X
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	class UNiagaraSystem *HitEffectSystemClass;

	// �x�C�u���[�h�̃R���g���[���[�^�C�v
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay")
	EBeybladeControllerType ControllerType = EBeybladeControllerType::None;

	// �V���[�g���ꂽ���ǂ���
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	bool bIsShot;

	// �R���g���[���[�����蓖�Ă�ꂽ���ǂ���
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	bool bIsControllerAssigned;

	// Stop�����s���ꂽ���ǂ���
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	bool bHasStopExecuted = false;

	// �x�C�u���[�h�̖��O
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay")
	FString BeybladeName = "NoName";

	// �Q�[���C���X�^���X�ւ̎Q��
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	class UBeybladeGameInstance *bbGameInstance;

	// ��]���x
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	float RotationSpeed = 15000.0f;

	// ���̃x�C�u���[�h�̈ړ����x
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	float MoveSpeed = 15000.0f;

	// ���̃x�C�u���[�h�̗̑�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	float HP = 100.0f;

	// ���̃x�C�u���[�h�̌��݂̃X�e�[�^�X
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	EBeybladeStatus Status;

	// �K�E�Z�̃^�C�g���e�N�X�`��
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Assets")
	UTexture2D *UltimateTitle;

	// ���̃x�C�u���[�h���u�[�X�g�����ǂ���
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	bool IsBoost;

	// �K�E�Z���g�p���ꂽ���ǂ���
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	bool IsUltimateUsed;

	// �K�E�Z�����܂ł̒x������
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float UltimateCastingDelay = 0.0f;

	// �V���[�g���Ƀx�C�u���[�h�ɉ�����
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float LaunchPower = 12000.0f;

	// �V���[�g�p�x�̍ő�l
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float ShootAngleMax = 1.7f;

	// �V���[�g�p�x�̍ŏ��l
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float ShootAngleMin = -1.7f;

	// �_���[�W��^����ۂ̌W��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float DamageCoefficient = 0.001f;

	// �P��̍U���ŗ^���邱�Ƃ��ł���ő�_���[�W��
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float MaxDamage = 15.0f;

	// �_���[�W���󂯂���̃N�[���_�E������
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float DamageCD = 0.2f;

	// �_���[�W���󂯂���̃N�[���_�E���^�C�}�[
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float DamageCDTimer = 0.f;

	// HP�̎��R�����Ɋւ���W��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float DurabilityCoefficient = 1.0f;

	// �\�����̏Ə����x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float AimSpeed = 0.1f;

	// �ő�HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float MaxHP;

	// �u�[�X�g�Q�[�W�̎c��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float Boost = 100.0f;

	// �u�[�X�g�Q�[�W�̍ő�l
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float BoostMax = 100.0f;

	// �u�[�X�g����1�b������̏����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float BoostCost = 20.0f;

	// �u�[�X�g�Q�[�W�̉񕜑��x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float BoostRegen = 10.0f;

	// �f���^�^�C����ۑ����邽�߂̕ϐ�
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float InternalDeltaTime = 0.0f;

	// �K�E�X�e�[�^�X�̎c�莞��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float UltimateTimer;

	// ��_���[�W���̌y���W��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float DamageReduction = 1.0f;

	// �V���[�g�O�̑ҋ@�ʒu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector StandbyPos;

	// �V���[�g�O�̏Ə�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector AimVector;

	// �\���p���b�V��
	UPROPERTY(EditDefaultsOnly, Category = "Assets")
	UStaticMesh *PredictionMesh;

	// �\���p���b�V���̃}�e���A��
	UPROPERTY(EditDefaultsOnly, Category = "Assets")
	UMaterial *PredictionMat;

	// �R���|�[�l���g
protected:
	// CapsuleComponent�R���|�[�l���g
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent *BeybladeCapsuleComponent;

	// StaticMeshComponent�R���|�[�l���g
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent *StaticMeshComponent;

		// StaticMeshComponent�R���|�[�l���g
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent *MeshPitchAndRollComponent;

	// NiagaraComponent�R���|�[�l���g
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraComponent *NiagaraComponent;

	// SplineComponent�R���|�[�l���g
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent *SplineComponent;

	// SplineMeshComponent�R���|�[�l���g�̔z��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class USplineMeshComponent *> SplineMeshComponentArray;

	// TGU_AxisLocker�R���|�[�l���g
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTGU_AxisLocker *AxisLocker;

	/** ���̓A�N�V���� */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction *BoostInput;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction *CameraInput;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction *LaunchInput;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction *MoveInput;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction *UltraInput;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction *PauseInput;
};