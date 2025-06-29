// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Beyblade_suzaku.cpp
// �鐝�x�C�u���[�h�̔h���N���X����
// �쐬���F2025/04/21

#include "Beyblade_suzaku.h"

// �R���X�g���N�^
ABeyblade_suzaku::ABeyblade_suzaku()
{
	// �x�C�u���[�h����ݒ�
	BeybladeName = TEXT("suzaku");
}

// �K�E�Z�𔭓�
void ABeyblade_suzaku::ActiveUltimate()
{
	// �K�E�Z�^�C�}�[��ݒ�
	UltimateTimer = 5.0f;
}

// �K�E�Z����Tick����
void ABeyblade_suzaku::UltimateTick(float Deltatime)
{
	// �K�E�Z���L���Ȋ�
	if ((Status == EBeybladeStatus::Ultra) && (UltimateTimer > 0.0f))
	{
		// HP�̎��R�����𔽓]�����A�񕜂�����
		DurabilityCoefficient = -1.5f;
		UltimateTimer -= Deltatime;
	}
	else
	{
		// �ʏ�̎��R�����W���ɖ߂�
		DurabilityCoefficient = 1.0f;
	}
}