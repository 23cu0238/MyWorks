// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// MyThumbnailSaver.cpp
// アセットのサムネイルを保存するエディタ機能の実装
// 作成日：2025/04/11

#include "MyThumbnailSaver.h"

// サムネイルを保存する関数
void UMyThumbnailSaver::SaveThumbnail(const FAssetData &AssetData, FString OutputPath)
{
// エディタ環境でのみ実行
#if WITH_EDITOR

	FString PackageFilename;
	const FName ObjectFullName = FName(*AssetData.GetFullName());
	TSet<FName> ObjectFullNames;
	ObjectFullNames.Add(ObjectFullName);

	// アセットのパッケージが存在するか確認
	if (FPackageName::DoesPackageExist(AssetData.PackageName.ToString(), &PackageFilename))
	{
		FThumbnailMap ThumbnailMap;
		// パッケージからサムネイルをロード
		ThumbnailTools::LoadThumbnailsFromPackage(PackageFilename, ObjectFullNames,
												  ThumbnailMap);

		// 目的のアセットのサムネイルを検索
		FObjectThumbnail *objTN = ThumbnailMap.Find(ObjectFullName);
		if (objTN)
		{
			// ImageWrapperモジュールを使用してPNG形式に変換
			IImageWrapperModule &ImageWrapperModule = FModuleManager::Get().LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));
			TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
			ImageWrapper->SetRaw(objTN->GetUncompressedImageData().GetData(), objTN->GetUncompressedImageData().Num(), objTN->GetImageWidth(), objTN->GetImageHeight(), ERGBFormat::BGRA, 8);
			if (ImageWrapper)
			{
				// 圧縮されたバイト配列を取得し、ファイルに保存
				const TArray64<uint8> &CompressedByteArray = ImageWrapper->GetCompressed();
				FFileHelper::SaveArrayToFile(CompressedByteArray, *OutputPath);
			}
		}
	}
#endif
}