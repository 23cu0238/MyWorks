// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Beyblade_genbu.cpp
// 玄武ベイブレードの派生クラス実装
// 作成日：2025/04/21

#include "Beyblade_genbu.h"

// コンストラクタ
ABeyblade_genbu::ABeyblade_genbu()
{
	// ベイブレード名を設定
	BeybladeName = TEXT("genbu");
}

// 必殺技を発動
void ABeyblade_genbu::ActiveUltimate()
{
	// 必殺技タイマーを設定
	UltimateTimer = 15.0f;
}

// 必殺技中のTick処理
void ABeyblade_genbu::UltimateTick(float Deltatime)
{
	// 必殺技が有効な間
	if ((Status == EBeybladeStatus::Ultra) && (UltimateTimer > 0.0f))
	{
		// 被ダメージを半減させる
		DamageReduction = 0.5f;
	}
	else
	{
		// 通常の自然減少係数に戻す
		DamageReduction = 1.0f;
	}
}