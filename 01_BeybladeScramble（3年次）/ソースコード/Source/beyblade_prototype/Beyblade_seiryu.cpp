// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Beyblade_seiryu.cpp
// 青龍ベイブレードの派生クラス実装
// 作成日：2025/04/21

#include "Beyblade_seiryu.h"

// コンストラクタ
ABeyblade_seiryu::ABeyblade_seiryu()
{
	// ベイブレード名を設定
	BeybladeName = TEXT("seiryu");
}

// 必殺技を発動
void ABeyblade_seiryu::ActiveUltimate()
{
	// 必殺技タイマーを設定
	UltimateTimer = 10.0f;
}

// 必殺技中のTick処理
void ABeyblade_seiryu::UltimateTick(float Deltatime)
{
	// 必殺技が有効な間
	if ((Status == EBeybladeStatus::Ultra) && (UltimateTimer > 0.0f))
	{
		// 移動速度を大幅に上昇させる
		MoveSpeed = 25000.0f;
	}
	else
	{
		// 通常の移動速度に戻す
		MoveSpeed = 15000.0f;
	}
}