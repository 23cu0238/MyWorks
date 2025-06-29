// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// MyThumbnailSaver.cpp
// �A�Z�b�g�̃T���l�C����ۑ�����G�f�B�^�@�\�̎���
// �쐬���F2025/04/11

#include "MyThumbnailSaver.h"

// �T���l�C����ۑ�����֐�
void UMyThumbnailSaver::SaveThumbnail(const FAssetData &AssetData, FString OutputPath)
{
// �G�f�B�^���ł̂ݎ��s
#if WITH_EDITOR

	FString PackageFilename;
	const FName ObjectFullName = FName(*AssetData.GetFullName());
	TSet<FName> ObjectFullNames;
	ObjectFullNames.Add(ObjectFullName);

	// �A�Z�b�g�̃p�b�P�[�W�����݂��邩�m�F
	if (FPackageName::DoesPackageExist(AssetData.PackageName.ToString(), &PackageFilename))
	{
		FThumbnailMap ThumbnailMap;
		// �p�b�P�[�W����T���l�C�������[�h
		ThumbnailTools::LoadThumbnailsFromPackage(PackageFilename, ObjectFullNames,
												  ThumbnailMap);

		// �ړI�̃A�Z�b�g�̃T���l�C��������
		FObjectThumbnail *objTN = ThumbnailMap.Find(ObjectFullName);
		if (objTN)
		{
			// ImageWrapper���W���[�����g�p����PNG�`���ɕϊ�
			IImageWrapperModule &ImageWrapperModule = FModuleManager::Get().LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));
			TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
			ImageWrapper->SetRaw(objTN->GetUncompressedImageData().GetData(), objTN->GetUncompressedImageData().Num(), objTN->GetImageWidth(), objTN->GetImageHeight(), ERGBFormat::BGRA, 8);
			if (ImageWrapper)
			{
				// ���k���ꂽ�o�C�g�z����擾���A�t�@�C���ɕۑ�
				const TArray64<uint8> &CompressedByteArray = ImageWrapper->GetCompressed();
				FFileHelper::SaveArrayToFile(CompressedByteArray, *OutputPath);
			}
		}
	}
#endif
}