// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// BeybladeGameInstance.cpp
// �Q�[���̏�Ԃ��Ǘ�����Q�[���C���X�^���X�̎���
// �쐬���F2025/04/18

#include "BeybladeGameInstance.h"
#include "EnvironmentManager.h"
#include "Kismet/GameplayStatics.h"

// �K�E�Z�̎g�p��Ԃ�ʒm���A���G�t�F�N�g��؂�ւ��܂�
void UBeybladeGameInstance::NotifyUltraStatus(ABeybladeBase *ultimateUser)
{
    // ���}�l�[�W���[�����擾�Ȃ�擾�����݂�
    if (!EnvironmentManager)
    {
        EnvironmentManager = Cast<AEnvironmentManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnvironmentManager::StaticClass()));
        if (!EnvironmentManager)
        {
            return;
        }
    }

    // ���ɕK�E�Z�g�p�҂�����ꍇ�i���K�E�Z�I�����j
    if (UltimateUser)
    {
        // ���C�g�G�t�F�N�g���I�t�ɂ��A�g�p�҂����Z�b�g
        EnvironmentManager->ToggleUltraLightEffect(false);
        UltimateUser = nullptr;
    }
    else // �K�E�Z�g�p�҂����Ȃ��ꍇ�i���K�E�Z�J�n���j
    {
        // ���C�g�G�t�F�N�g���I���ɂ��A�g�p�҂�ݒ�
        EnvironmentManager->ToggleUltraLightEffect(true);
        UltimateUser = ultimateUser;
    }
}