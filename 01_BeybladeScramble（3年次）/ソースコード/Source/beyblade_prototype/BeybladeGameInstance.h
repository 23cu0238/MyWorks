// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// BeybladeGameInstance.h
// �Q�[���̏�Ԃ��Ǘ�����Q�[���C���X�^���X�̃w�b�_
// �쐬���F2025/04/18

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BeybladeBase.h"
#include "BeybladeGameInstance.generated.h"

/**
 * �Q�[���S�̂ŕێ������C���X�^���X�N���X
 */
UCLASS()
class BEYBLADE_PROTOTYPE_API UBeybladeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// �K�E�Z�̎g�p��Ԃ�ʒm���A���G�t�F�N�g��؂�ւ��܂�
	UFUNCTION(BlueprintCallable)
	void NotifyUltraStatus(ABeybladeBase *ultimateUser);

public:
	// �Q�[���ɎQ�����Ă���x�C�u���[�h�̃��X�g
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int32, ABeybladeBase *> BeybladeList;

	// ���}�l�[�W���[�ւ̎Q��
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<class AEnvironmentManager> EnvironmentManager;

	// ���ݕK�E�Z���g�p���Ă��郆�[�U�[���擾���܂�
	UFUNCTION(BlueprintCallable)
	ABeybladeBase *GetUltimateUser() { return UltimateUser; }

private:
	// �K�E�Z���g�p���Ă��郆�[�U�[
	UPROPERTY()
	ABeybladeBase *UltimateUser;
};