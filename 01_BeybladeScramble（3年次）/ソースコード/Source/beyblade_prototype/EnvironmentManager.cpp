// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// EnvironmentManager.cpp
// ���C�e�B���O�ȂǊ����ʂ��Ǘ�����N���X�̎���
// �쐬���F2025/04/18

#include "EnvironmentManager.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SpotLight.h"
#include "Components/LightComponent.h"
#include "Kismet/GameplayStatics.h"

// �f�t�H���g�l��ݒ肵�܂�
AEnvironmentManager::AEnvironmentManager()
{
    // ���̃A�N�^�����t���[��Tick()���Ăяo���悤�ɐݒ肵�܂��B�p�t�H�[�}���X����̂��߂ɕs�v�ȏꍇ�̓I�t�ɂł��܂��B
    PrimaryActorTick.bCanEverTick = true;
}

// �Q�[���J�n���܂��̓X�|�[�����ɌĂ΂�܂�
void AEnvironmentManager::BeginPlay()
{
    Super::BeginPlay();

    // ���[���h�ɑ��݂���f�B���N�V���i�����C�g�ƃX�|�b�g���C�g���擾���܂�
    EnvironmentLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
    SpotLight = Cast<ASpotLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpotLight::StaticClass()));
}

// ���t���[���Ă΂�܂�
void AEnvironmentManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// �K�E�Z�̃��C�g�G�t�F�N�g��؂�ւ��܂�
void AEnvironmentManager::ToggleUltraLightEffect(bool IsOn)
{
    // ���C�g���I���ɂ���ꍇ
    if (IsOn)
    {
        // �������Â�����
        if (EnvironmentLight)
        {
            EnvironmentLight->GetLightComponent()->SetIntensity(0.01f);
        }

        // �X�|�b�g���C�g�𖾂邭����
        if (SpotLight)
        {
            SpotLight->GetLightComponent()->SetIntensity(3.0f);
        }

        // �Q�[���̎��Ԃ��X���[�ɂ���
        UGameplayStatics::SetGlobalTimeDilation(this, 0.01f);

        IsUltraLightOn = true;
    }
    else // ���C�g���I�t�ɂ���ꍇ
    {
        // ���������̖��邳�ɖ߂�
        if (EnvironmentLight)
        {
            EnvironmentLight->GetLightComponent()->SetIntensity(10.0f);
        }

        // �X�|�b�g���C�g������
        if (SpotLight)
        {
            SpotLight->GetLightComponent()->SetIntensity(0.0f);
        }

        // �Q�[���̎��Ԃ����ɖ߂�
        UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);

        IsUltraLightOn = false;
    }
}

// �X�|�b�g���C�g���w�肵���ʒu�Ɉړ������܂�
void AEnvironmentManager::MoveSpotlight(const FVector NewPosition)
{
    if (!SpotLight)
        return;

    // �X�|�b�g���C�g�̃A�N�^�ʒu���X�V
    SpotLight->SetActorLocation(NewPosition, false, nullptr, ETeleportType::None);
}