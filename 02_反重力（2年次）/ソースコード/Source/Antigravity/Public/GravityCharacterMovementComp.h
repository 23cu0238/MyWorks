// ------------------------------------------------
// GravityCharacterMovementComp.h
// �d�̓L�����N�^�[�̃��[�u�����g�R���|�[�l���g�N���X
// �쐬�ҁF���N�F
// �쐬�����F2024-12-01
// ------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GravityCharacterMovementComp.generated.h"

// �O���錾
class ACharacter;

UCLASS()
class ANTIGRAVITY_API UGravityCharacterMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	/** ���t���[���X�V����Ƃ����[�e�[�V�������X�V */
	void PhysicsRotation(float DeltaTime) override;

	// �X�^�[�g�֐�
	virtual void BeginPlay() override;

	// ���t���[���X�V
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	// ��]�ʂ̍����擾
	UFUNCTION(BlueprintCallable)
	FRotator GetDeltaRot() const { return mDeltaRot; }

	// �d�͕�����ύX
	UFUNCTION(BlueprintCallable)
	void SetGravityDirP(const FVector &GravityDir) { SetGravityDirection(GravityDir); }

	// �n�ʂɐ������ׂ����H
	UFUNCTION(BlueprintCallable)
	bool GetShouldRemainVertical() const
	{
		if (!bRemainVertical)
		{
			return false;
		}

		else
		{
			return ShouldRemainVertical();
		}
	}

private:
	// �����̉�]��
	FRotator mOriginalRotationRate;

	// �n�ʂɐ������ׂ�����ݒ�p�ϐ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gravity, meta = (AllowPrivateAccess = "true"))
	bool bRemainVertical;

	// ��]�ʂ̍�
	FRotator mDeltaRot;
};
