// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Beyblade_genbu.cpp
// �����x�C�u���[�h�̔h���N���X����
// �쐬���F2025/04/21

#include "Beyblade_genbu.h"

// �R���X�g���N�^
ABeyblade_genbu::ABeyblade_genbu()
{
	// �x�C�u���[�h����ݒ�
	BeybladeName = TEXT("genbu");
}

// �K�E�Z�𔭓�
void ABeyblade_genbu::ActiveUltimate()
{
	// �K�E�Z�^�C�}�[��ݒ�
	UltimateTimer = 15.0f;
}

// �K�E�Z����Tick����
void ABeyblade_genbu::UltimateTick(float Deltatime)
{
	// �K�E�Z���L���Ȋ�
	if ((Status == EBeybladeStatus::Ultra) && (UltimateTimer > 0.0f))
	{
		// ��_���[�W�𔼌�������
		DamageReduction = 0.5f;
	}
	else
	{
		// �ʏ�̎��R�����W���ɖ߂�
		DamageReduction = 1.0f;
	}
}