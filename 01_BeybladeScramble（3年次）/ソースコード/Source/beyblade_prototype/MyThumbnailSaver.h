// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// MyThumbnailSaver.h
// �A�Z�b�g�̃T���l�C����ۑ�����G�f�B�^�@�\�̃w�b�_
// �쐬���F2025/04/11

#pragma once

// �G�f�B�^���ł݈̂ȉ��̃w�b�_���C���N���[�h
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
 * �A�Z�b�g�̃T���l�C�����t�@�C���ɕۑ����邽�߂̃��[�e�B���e�B�N���X
 */
struct FAssetData;

UCLASS()
class BEYBLADE_PROTOTYPE_API UMyThumbnailSaver : public UObject
{
	GENERATED_BODY()

public:
	// �A�Z�b�g�f�[�^����T���l�C���𒊏o���A�w�肳�ꂽ�p�X�ɕۑ����܂�
	UFUNCTION(BlueprintCallable)
	static void SaveThumbnail(const FAssetData &AssetData, FString OutputPath);
};