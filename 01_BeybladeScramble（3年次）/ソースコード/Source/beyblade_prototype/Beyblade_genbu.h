// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Beyblade_genbu.h
// �����x�C�u���[�h�̔h���N���X�w�b�_
// �쐬���F2025/04/21

#pragma once

#include "CoreMinimal.h"
#include "BeybladeBase.h"
#include "Beyblade_genbu.generated.h"

/**
 * �������`�[�t�̃x�C�u���[�h�N���X
 */
UCLASS()
class BEYBLADE_PROTOTYPE_API ABeyblade_genbu : public ABeybladeBase
{
	GENERATED_BODY()

	// �R���X�g���N�^
	ABeyblade_genbu();

protected:
	// �K�E�Z�𔭓����܂�
	UFUNCTION(BlueprintCallable)
	void ActiveUltimate() override;

	// �K�E�Z����Tick����
	UFUNCTION(BlueprintCallable)
	void UltimateTick(float Deltatime) override;
};