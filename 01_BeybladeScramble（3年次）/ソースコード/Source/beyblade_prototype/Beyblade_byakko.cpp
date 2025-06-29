// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Beyblade_byakko.cpp
// 白虎ベイブレードの派生クラス実装
// 作成日：2025/04/21

#include "Beyblade_byakko.h"

// コンストラクタ
ABeyblade_byakko::ABeyblade_byakko()
{
    // ベイブレード名を設定
    BeybladeName = TEXT("byakko");
}

// 必殺技を発動
void ABeyblade_byakko::ActiveUltimate()
{
    // 必殺技タイマーを設定（非常に長い時間、事実上次のヒットまで持続）
    UltimateTimer = 1000.0f;
}

// 必殺技中のTick処理
void ABeyblade_byakko::UltimateTick(float Deltatime)
{
    // 必殺技が有効な間
    if ((Status == EBeybladeStatus::Ultra) && (UltimateTimer > 0.0f))
    {
        // ダメージ係数を最大ダメージ値に設定し、一撃必殺状態にする
        DamageCoefficient = MaxDamage;
    }
    else
    {
        // 通常のダメージ係数に戻す
        DamageCoefficient = 0.001f;
    }
}

// ダメージを与えたときの処理
void ABeyblade_byakko::OnDamageDealt()
{
    // 必殺技中でなければ何もしない
    if (Status != EBeybladeStatus::Ultra)
        return;

    // ダメージを与えたら必殺技タイマーを0にして効果を終了させる
    UltimateTimer = 0.0f;
}