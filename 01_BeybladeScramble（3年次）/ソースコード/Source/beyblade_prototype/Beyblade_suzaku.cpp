// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Beyblade_suzaku.cpp
// 朱雀ベイブレードの派生クラス実装
// 作成日：2025/04/21

#include "Beyblade_suzaku.h"

// コンストラクタ
ABeyblade_suzaku::ABeyblade_suzaku()
{
	// ベイブレード名を設定
	BeybladeName = TEXT("suzaku");
}

// 必殺技を発動
void ABeyblade_suzaku::ActiveUltimate()
{
	// 必殺技タイマーを設定
	UltimateTimer = 5.0f;
}

// 必殺技中のTick処理
void ABeyblade_suzaku::UltimateTick(float Deltatime)
{
	// 必殺技が有効な間
	if ((Status == EBeybladeStatus::Ultra) && (UltimateTimer > 0.0f))
	{
		// HPの自然減少を反転させ、回復させる
		DurabilityCoefficient = -1.5f;
		UltimateTimer -= Deltatime;
	}
	else
	{
		// 通常の自然減少係数に戻す
		DurabilityCoefficient = 1.0f;
	}
}