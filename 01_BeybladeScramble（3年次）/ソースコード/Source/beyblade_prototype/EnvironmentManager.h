// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// EnvironmentManager.h
// ���C�e�B���O�ȂǊ����ʂ��Ǘ�����N���X�̃w�b�_
// �쐬���F2025/04/18

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnvironmentManager.generated.h"

UCLASS()
class BEYBLADE_PROTOTYPE_API AEnvironmentManager : public AActor
{
	GENERATED_BODY()

public:
	// ���̃A�N�^�̃v���p�e�B�̃f�t�H���g�l��ݒ肵�܂�
	AEnvironmentManager();

protected:
	// �Q�[���J�n���܂��̓X�|�[�����ɌĂ΂�܂�
	virtual void BeginPlay() override;

public:
	// ���t���[���Ă΂�܂�
	virtual void Tick(float DeltaTime) override;

	// �K�E�Z���g�����̃��C�g�̃��[�h�؂�ւ�
	UFUNCTION(BlueprintCallable, Category = "Light")
	void ToggleUltraLightEffect(bool IsOn);

	// �K�E�Z���C�g�G�t�F�N�g���I�����ǂ������擾���܂�
	UFUNCTION(BlueprintCallable, Category = "Light")
	bool GetIsUltraLightOn() const { return IsUltraLightOn; }

	// �X�|�b�g���C�g���w�肵���ʒu�Ɉړ������܂�
	UFUNCTION(BlueprintCallable, Category = "Light")
	void MoveSpotlight(const FVector NewPosition);

private:
	// �����i�f�B���N�V���i�����C�g�j�ւ̎Q��
	UPROPERTY()
	class ADirectionalLight *EnvironmentLight;

	// ���o�p�i�X�|�b�g���C�g�j�ւ̎Q��
	UPROPERTY()
	class ASpotLight *SpotLight;

	// �K�E�Z���C�g�G�t�F�N�g���I�����ǂ���
	UPROPERTY()
	bool IsUltraLightOn;
};