// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// BeybladeBlueprintFunctionLibrary.cpp
// �u���[�v�����g����Ăяo����֗��֐��̎���
// �쐬���F2025/04/11

#include "BeybladeBlueprintFunctionLibrary.h"
#include "GameMapsSettings.h"
#include "GameFramework/PlayerController.h"

// �Q�[���p�b�hID�̃I�t�Z�b�g��ݒ�/�������܂�
void UBeybladeBlueprintFunctionLibrary::OffsetGamepadID(bool Offset)
{
	UGameMapsSettings *Settings = const_cast<UGameMapsSettings *>(GetDefault<UGameMapsSettings>());

	if (Settings == nullptr)
	{
		return;
	}

	Settings->bOffsetPlayerGamepadIds = Offset;
}

// ���[�J���v���C���[�̃R���g���[���[ID��ݒ肵�܂�
void UBeybladeBlueprintFunctionLibrary::SetLocalPlayerControllerId(APlayerController *controller, int32 newId)
{
	if (controller)
	{
		controller->GetLocalPlayer()->SetControllerId(newId);
	}
}