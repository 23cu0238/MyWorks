// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// BeybladeBlueprintFunctionLibrary.h
// �u���[�v�����g����Ăяo����֗��֐��̃w�b�_
// �쐬���F2025/04/11

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BeybladeBlueprintFunctionLibrary.generated.h"

/**
 * */
class APlayerController;

UCLASS()
class BEYBLADE_PROTOTYPE_API UBeybladeBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// UGameMapSettings�� "bOffsetPlayerGamepadIds" ��ݒ肵�܂�
	UFUNCTION(BlueprintCallable, Category = "Gamepad")
	static void OffsetGamepadID(bool Offset); // ���C�u�����̂��ߐÓI�֐�

	// ���[�J���v���C���[�̃R���g���[���[ID��ݒ肵�܂�
	UFUNCTION(BlueprintCallable, Category = "Gamepad")
	static void SetLocalPlayerControllerId(APlayerController *controller, int32 newId); // ���C�u�����̂��ߐÓI�֐�

	// ���A���^�C���̃f���^�b���擾���܂��i�|�[�Y���̉e�����󂯂Ȃ��j
	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static float GetRealTimeDeltaSeconds(UGameInstance *gameInstance) { return gameInstance->GetWorld()->GetTime().GetDeltaRealTimeSeconds(); } // ���C�u�����̂��ߐÓI�֐�
};