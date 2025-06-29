// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// BeybladeGameInstance.h
// ゲームの状態を管理するゲームインスタンスのヘッダ
// 作成日：2025/04/18

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BeybladeBase.h"
#include "BeybladeGameInstance.generated.h"

/**
 * ゲーム全体で保持されるインスタンスクラス
 */
UCLASS()
class BEYBLADE_PROTOTYPE_API UBeybladeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 必殺技の使用状態を通知し、環境エフェクトを切り替えます
	UFUNCTION(BlueprintCallable)
	void NotifyUltraStatus(ABeybladeBase *ultimateUser);

public:
	// ゲームに参加しているベイブレードのリスト
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int32, ABeybladeBase *> BeybladeList;

	// 環境マネージャーへの参照
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<class AEnvironmentManager> EnvironmentManager;

	// 現在必殺技を使用しているユーザーを取得します
	UFUNCTION(BlueprintCallable)
	ABeybladeBase *GetUltimateUser() { return UltimateUser; }

private:
	// 必殺技を使用しているユーザー
	UPROPERTY()
	ABeybladeBase *UltimateUser;
};