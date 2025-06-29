// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Beyblade_genbu.h
// 玄武ベイブレードの派生クラスヘッダ
// 作成日：2025/04/21

#pragma once

#include "CoreMinimal.h"
#include "BeybladeBase.h"
#include "Beyblade_genbu.generated.h"

/**
 * 玄武モチーフのベイブレードクラス
 */
UCLASS()
class BEYBLADE_PROTOTYPE_API ABeyblade_genbu : public ABeybladeBase
{
	GENERATED_BODY()

	// コンストラクタ
	ABeyblade_genbu();

protected:
	// 必殺技を発動します
	UFUNCTION(BlueprintCallable)
	void ActiveUltimate() override;

	// 必殺技中のTick処理
	UFUNCTION(BlueprintCallable)
	void UltimateTick(float Deltatime) override;
};