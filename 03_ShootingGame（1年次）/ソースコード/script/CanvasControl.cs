// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// CanvasControl.cs
// UI�L�����o�X��̗v�f�i�{�XHP�o�[�Ȃǁj�̕\���𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CanvasControl : MonoBehaviour
{
    public GameObject BossHP; // �{�X��HP�o�[UI�I�u�W�F�N�g�ւ̎Q��

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // �����������������ɒǉ��ł��܂�
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �X�V�����������ɒǉ��ł��܂�
    }

    // �{�XHP�o�[��L�����i�\���j����֐�
    public void ActivateHPBar()
    {
        if(BossHP != null)
        {
            BossHP.SetActive(true);
        }
    }    
}