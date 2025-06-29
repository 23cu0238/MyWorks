// ------------------------------------------------
// GravityController.h
// �R���g���[���N���X
// �쐬�ҁF���N�F
// �쐬�����F2024-11-28
// ------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GravityController.generated.h"

/**
 * A Player Controller class which adds input-handling functionality for
 * CharacterMovementController's custom gravity mechanics.
 */
UCLASS()
class AGravityController : public APlayerController
{
	GENERATED_BODY()

public:
	// ��]�̍X�V
	virtual void UpdateRotation(float DeltaTime) override;

	// ��]�𐢊E��Ԃ���d�͑��΋�Ԃɕϊ�����
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityRelativeRotation(FRotator Rotation, FVector GravityDirection);

	// �d�͑��΋�Ԃ��琢�E��Ԃɉ�]��ϊ����܂�
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityWorldRotation(FRotator Rotation, FVector GravityDirection);

private:
	// �P�t���[���O�̏d�͕���
	FVector LastFrameGravity = FVector::ZeroVector;
};