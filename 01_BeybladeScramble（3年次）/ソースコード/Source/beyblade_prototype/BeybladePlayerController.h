// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// BeybladePlayerController.h
// プレイヤーの入力を処理するコントローラーのヘッダ
// 作成日：2025/04/28

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BeybladePlayerController.generated.h"

/**
 * プレイヤーからの入力を処理するコントローラークラス
 */
UCLASS()
class BEYBLADE_PROTOTYPE_API ABeybladePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// このコントローラーにマッピングコンテキストを追加します
	UFUNCTION()
	virtual void AddMappingContextForController();

	// ゲーム開始時に呼ばれます
	UFUNCTION()
	virtual void BeginPlay() override;

	// 毎フレーム呼ばれます
	UFUNCTION()
	virtual void Tick(float DeltaTime) override;

private:
	// 使用する入力マッピングコンテキスト
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SerializeField", meta = (AllowPrivateAccess = true))
	class UInputMappingContext *InputMappingContext;
};