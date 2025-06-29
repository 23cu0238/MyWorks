//	 �d�͂�ύX����̂Ɏg���܂�
//	���łȂ̂ŃN���[���͎󂯕t���܂���
//	Component�N���X

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChangeGravityComponent.generated.h"

class ACharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ANTIGRAVITY_API UChangeGravityComponent : public UActorComponent
{
	// UE�ŗL
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	UChangeGravityComponent();

protected:
	// ������
	virtual void BeginPlay() override;

public:
	// �d�͂�ς���Ώۂ�ݒ肵�܂�
	// ����1 : Target�̃A�h���X
	void SetTarget(ACharacter *target);
	// �d�͂�ύX������s�֐�
	// ����1 : �d�͂̕���
	void ChangeGravity(const FHitResult &gravityPoint);
	// �ύX���I�������ĂԂ���
	void OnExit();

private:
	// �d�͂�ύX������Ώ�
	ACharacter *mTarget;
};
