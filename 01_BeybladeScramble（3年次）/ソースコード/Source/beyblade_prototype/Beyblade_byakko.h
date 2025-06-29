// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Beyblade_byakko.h
// 白虎ベイブレードの派生クラスヘッダ
// 作成日：2025/04/21

#pragma once

#include "CoreMinimal.h"
#include "BeybladeBase.h"
#include "Beyblade_byakko.generated.h"

/**
 * 白虎モチーフのベイブレードクラス
 */
UCLASS()
class BEYBLADE_PROTOTYPE_API ABeyblade_byakko : public ABeybladeBase
{
	GENERATED_BODY()

	// コンストラクタ
	ABeyblade_byakko();

protected:
	// 必殺技を発動します
	UFUNCTION(BlueprintCallable)
	void ActiveUltimate() override;

	// 必殺技中のTick処理
	UFUNCTION(BlueprintCallable)
	void UltimateTick(float Deltatime) override;

	// ダメージを与えたときの処理
	UFUNCTION(BlueprintCallable)
	void OnDamageDealt() override;
};