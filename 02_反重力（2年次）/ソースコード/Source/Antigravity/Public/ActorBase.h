// ------------------------------------------------
// ActorBase.h
// �L�����N�^�[�̊�{�N���X
// �쐬�ҁF���N�F�A���ᩗ�
// �쐬�����F2024-11-18
// ------------------------------------------------

#pragma once
#include "IDeadable.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActorBase.generated.h"

// �O���錾
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ANTIGRAVITY_API AActorBase : public ACharacter, public IDeadable
{
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	AActorBase(const FObjectInitializer &ObjectInitializer);

protected:
	// �X�^�[�g�֐�
	virtual void BeginPlay() override;

public:
	// ������
	virtual void Init();
	// ���s�֐�
	virtual void Update() {};

public:
	// ���t���[���X�V
	virtual void Tick(float DeltaTime) override;

	// ���S����֐�
	bool Dead() override;

	// HP���炷�֐�
	void FluctuationHP(float decrease) override;

	// HP���擾�p
	UFUNCTION(BlueprintCallable)
	int GetHP() const { return mHitPoint; }

protected:
	// ���x
	float mMass;
	// �ړ����x
	float mMoveSpeed;
	// ���G���ԃ^�C�}�[
	float mDamageTimer;
	// ���G����
	float mDamageCD;
	// �̗�
	UPROPERTY(BlueprintReadWrite, Category = Parameter, meta = (AllowPrivateAccess = "true"))
	int mHitPoint;
};