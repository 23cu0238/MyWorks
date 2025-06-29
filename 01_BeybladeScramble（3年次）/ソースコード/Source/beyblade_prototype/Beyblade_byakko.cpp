// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Beyblade_byakko.cpp
// ���Ճx�C�u���[�h�̔h���N���X����
// �쐬���F2025/04/21

#include "Beyblade_byakko.h"

// �R���X�g���N�^
ABeyblade_byakko::ABeyblade_byakko()
{
    // �x�C�u���[�h����ݒ�
    BeybladeName = TEXT("byakko");
}

// �K�E�Z�𔭓�
void ABeyblade_byakko::ActiveUltimate()
{
    // �K�E�Z�^�C�}�[��ݒ�i���ɒ������ԁA�����㎟�̃q�b�g�܂Ŏ����j
    UltimateTimer = 1000.0f;
}

// �K�E�Z����Tick����
void ABeyblade_byakko::UltimateTick(float Deltatime)
{
    // �K�E�Z���L���Ȋ�
    if ((Status == EBeybladeStatus::Ultra) && (UltimateTimer > 0.0f))
    {
        // �_���[�W�W�����ő�_���[�W�l�ɐݒ肵�A�ꌂ�K�E��Ԃɂ���
        DamageCoefficient = MaxDamage;
    }
    else
    {
        // �ʏ�̃_���[�W�W���ɖ߂�
        DamageCoefficient = 0.001f;
    }
}

// �_���[�W��^�����Ƃ��̏���
void ABeyblade_byakko::OnDamageDealt()
{
    // �K�E�Z���łȂ���Ή������Ȃ�
    if (Status != EBeybladeStatus::Ultra)
        return;

    // �_���[�W��^������K�E�Z�^�C�}�[��0�ɂ��Č��ʂ��I��������
    UltimateTimer = 0.0f;
}