// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// BeybladeGameInstance.cpp
// ゲームの状態を管理するゲームインスタンスの実装
// 作成日：2025/04/18

#include "BeybladeGameInstance.h"
#include "EnvironmentManager.h"
#include "Kismet/GameplayStatics.h"

// 必殺技の使用状態を通知し、環境エフェクトを切り替えます
void UBeybladeGameInstance::NotifyUltraStatus(ABeybladeBase *ultimateUser)
{
    // 環境マネージャーが未取得なら取得を試みる
    if (!EnvironmentManager)
    {
        EnvironmentManager = Cast<AEnvironmentManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnvironmentManager::StaticClass()));
        if (!EnvironmentManager)
        {
            return;
        }
    }

    // 既に必殺技使用者がいる場合（＝必殺技終了時）
    if (UltimateUser)
    {
        // ライトエフェクトをオフにし、使用者をリセット
        EnvironmentManager->ToggleUltraLightEffect(false);
        UltimateUser = nullptr;
    }
    else // 必殺技使用者がいない場合（＝必殺技開始時）
    {
        // ライトエフェクトをオンにし、使用者を設定
        EnvironmentManager->ToggleUltraLightEffect(true);
        UltimateUser = ultimateUser;
    }
}