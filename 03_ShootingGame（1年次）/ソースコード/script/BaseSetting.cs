// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// BaseSetting.cs
// �Q�[���̊�{�I�Ȑݒ�i�t���[�����[�g�Ȃǁj���s���N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BaseSetting : MonoBehaviour
{
    // �I�u�W�F�N�g���L���������O�́A�ŏ��̃t���[���X�V�O�ɌĂяo����܂�
	private void Awake()
	{
		// �����������I�t�ɂ���
		QualitySettings.vSyncCount = 0;
		// �ڕW�t���[�����[�g��60�ɐݒ�
		Application.targetFrameRate = 60;
	}
}