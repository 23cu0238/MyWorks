// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// BeybladePlayerController.h
// �v���C���[�̓��͂���������R���g���[���[�̃w�b�_
// �쐬���F2025/04/28

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BeybladePlayerController.generated.h"

/**
 * �v���C���[����̓��͂���������R���g���[���[�N���X
 */
UCLASS()
class BEYBLADE_PROTOTYPE_API ABeybladePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// ���̃R���g���[���[�Ƀ}�b�s���O�R���e�L�X�g��ǉ����܂�
	UFUNCTION()
	virtual void AddMappingContextForController();

	// �Q�[���J�n���ɌĂ΂�܂�
	UFUNCTION()
	virtual void BeginPlay() override;

	// ���t���[���Ă΂�܂�
	UFUNCTION()
	virtual void Tick(float DeltaTime) override;

private:
	// �g�p������̓}�b�s���O�R���e�L�X�g
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SerializeField", meta = (AllowPrivateAccess = true))
	class UInputMappingContext *InputMappingContext;
};