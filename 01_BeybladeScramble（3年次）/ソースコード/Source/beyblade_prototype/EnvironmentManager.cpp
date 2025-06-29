// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// EnvironmentManager.cpp
// ライティングなど環境効果を管理するクラスの実装
// 作成日：2025/04/18

#include "EnvironmentManager.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SpotLight.h"
#include "Components/LightComponent.h"
#include "Kismet/GameplayStatics.h"

// デフォルト値を設定します
AEnvironmentManager::AEnvironmentManager()
{
    // このアクタが毎フレームTick()を呼び出すように設定します。パフォーマンス向上のために不要な場合はオフにできます。
    PrimaryActorTick.bCanEverTick = true;
}

// ゲーム開始時またはスポーン時に呼ばれます
void AEnvironmentManager::BeginPlay()
{
    Super::BeginPlay();

    // ワールドに存在するディレクショナルライトとスポットライトを取得します
    EnvironmentLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
    SpotLight = Cast<ASpotLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpotLight::StaticClass()));
}

// 毎フレーム呼ばれます
void AEnvironmentManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// 必殺技のライトエフェクトを切り替えます
void AEnvironmentManager::ToggleUltraLightEffect(bool IsOn)
{
    // ライトをオンにする場合
    if (IsOn)
    {
        // 環境光を暗くする
        if (EnvironmentLight)
        {
            EnvironmentLight->GetLightComponent()->SetIntensity(0.01f);
        }

        // スポットライトを明るくする
        if (SpotLight)
        {
            SpotLight->GetLightComponent()->SetIntensity(3.0f);
        }

        // ゲームの時間をスローにする
        UGameplayStatics::SetGlobalTimeDilation(this, 0.01f);

        IsUltraLightOn = true;
    }
    else // ライトをオフにする場合
    {
        // 環境光を元の明るさに戻す
        if (EnvironmentLight)
        {
            EnvironmentLight->GetLightComponent()->SetIntensity(10.0f);
        }

        // スポットライトを消す
        if (SpotLight)
        {
            SpotLight->GetLightComponent()->SetIntensity(0.0f);
        }

        // ゲームの時間を元に戻す
        UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);

        IsUltraLightOn = false;
    }
}

// スポットライトを指定した位置に移動させます
void AEnvironmentManager::MoveSpotlight(const FVector NewPosition)
{
    if (!SpotLight)
        return;

    // スポットライトのアクタ位置を更新
    SpotLight->SetActorLocation(NewPosition, false, nullptr, ETeleportType::None);
}