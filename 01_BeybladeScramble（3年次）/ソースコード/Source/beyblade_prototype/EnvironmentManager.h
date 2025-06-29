// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// EnvironmentManager.h
// ライティングなど環境効果を管理するクラスのヘッダ
// 作成日：2025/04/18

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnvironmentManager.generated.h"

UCLASS()
class BEYBLADE_PROTOTYPE_API AEnvironmentManager : public AActor
{
	GENERATED_BODY()

public:
	// このアクタのプロパティのデフォルト値を設定します
	AEnvironmentManager();

protected:
	// ゲーム開始時またはスポーン時に呼ばれます
	virtual void BeginPlay() override;

public:
	// 毎フレーム呼ばれます
	virtual void Tick(float DeltaTime) override;

	// 必殺技を使う時のライトのモード切り替え
	UFUNCTION(BlueprintCallable, Category = "Light")
	void ToggleUltraLightEffect(bool IsOn);

	// 必殺技ライトエフェクトがオンかどうかを取得します
	UFUNCTION(BlueprintCallable, Category = "Light")
	bool GetIsUltraLightOn() const { return IsUltraLightOn; }

	// スポットライトを指定した位置に移動させます
	UFUNCTION(BlueprintCallable, Category = "Light")
	void MoveSpotlight(const FVector NewPosition);

private:
	// 環境光（ディレクショナルライト）への参照
	UPROPERTY()
	class ADirectionalLight *EnvironmentLight;

	// 演出用（スポットライト）への参照
	UPROPERTY()
	class ASpotLight *SpotLight;

	// 必殺技ライトエフェクトがオンかどうか
	UPROPERTY()
	bool IsUltraLightOn;
};