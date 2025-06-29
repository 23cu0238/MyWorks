// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// MyThumbnailSaver.h
// アセットのサムネイルを保存するエディタ機能のヘッダ
// 作成日：2025/04/11

#pragma once

// エディタ環境でのみ以下のヘッダをインクルード
#if WITH_EDITOR
#include "UnrealEd.h"
#include "ObjectTools.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "AssetRegistry/AssetRegistryModule.h"
#endif

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyThumbnailSaver.generated.h"

/**
 * アセットのサムネイルをファイルに保存するためのユーティリティクラス
 */
struct FAssetData;

UCLASS()
class BEYBLADE_PROTOTYPE_API UMyThumbnailSaver : public UObject
{
	GENERATED_BODY()

public:
	// アセットデータからサムネイルを抽出し、指定されたパスに保存します
	UFUNCTION(BlueprintCallable)
	static void SaveThumbnail(const FAssetData &AssetData, FString OutputPath);
};