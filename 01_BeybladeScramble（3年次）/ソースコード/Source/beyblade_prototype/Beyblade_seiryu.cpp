// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Beyblade_seiryu.cpp
// ���x�C�u���[�h�̔h���N���X����
// �쐬���F2025/04/21

#include "Beyblade_seiryu.h"

// �R���X�g���N�^
ABeyblade_seiryu::ABeyblade_seiryu()
{
	// �x�C�u���[�h����ݒ�
	BeybladeName = TEXT("seiryu");
}

// �K�E�Z�𔭓�
void ABeyblade_seiryu::ActiveUltimate()
{
	// �K�E�Z�^�C�}�[��ݒ�
	UltimateTimer = 10.0f;
}

// �K�E�Z����Tick����
void ABeyblade_seiryu::UltimateTick(float Deltatime)
{
	// �K�E�Z���L���Ȋ�
	if ((Status == EBeybladeStatus::Ultra) && (UltimateTimer > 0.0f))
	{
		// �ړ����x��啝�ɏ㏸������
		MoveSpeed = 25000.0f;
	}
	else
	{
		// �ʏ�̈ړ����x�ɖ߂�
		MoveSpeed = 15000.0f;
	}
}