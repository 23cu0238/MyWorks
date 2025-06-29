// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// BeybladeBlueprintFunctionLibrary.h
// ブループリントから呼び出せる便利関数のヘッダ
// 作成日：2025/04/11

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
	// UGameMapSettingsの "bOffsetPlayerGamepadIds" を設定します
	UFUNCTION(BlueprintCallable, Category = "Gamepad")
	static void OffsetGamepadID(bool Offset); // ライブラリのため静的関数

	// ローカルプレイヤーのコントローラーIDを設定します
	UFUNCTION(BlueprintCallable, Category = "Gamepad")
	static void SetLocalPlayerControllerId(APlayerController *controller, int32 newId); // ライブラリのため静的関数

	// リアルタイムのデルタ秒を取得します（ポーズ等の影響を受けない）
	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static float GetRealTimeDeltaSeconds(UGameInstance *gameInstance) { return gameInstance->GetWorld()->GetTime().GetDeltaRealTimeSeconds(); } // ライブラリのため静的関数
};