// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// BeybladeBlueprintFunctionLibrary.cpp
// ブループリントから呼び出せる便利関数の実装
// 作成日：2025/04/11

#include "BeybladeBlueprintFunctionLibrary.h"
#include "GameMapsSettings.h"
#include "GameFramework/PlayerController.h"

// ゲームパッドIDのオフセットを設定/解除します
void UBeybladeBlueprintFunctionLibrary::OffsetGamepadID(bool Offset)
{
	UGameMapsSettings *Settings = const_cast<UGameMapsSettings *>(GetDefault<UGameMapsSettings>());

	if (Settings == nullptr)
	{
		return;
	}

	Settings->bOffsetPlayerGamepadIds = Offset;
}

// ローカルプレイヤーのコントローラーIDを設定します
void UBeybladeBlueprintFunctionLibrary::SetLocalPlayerControllerId(APlayerController *controller, int32 newId)
{
	if (controller)
	{
		controller->GetLocalPlayer()->SetControllerId(newId);
	}
}