// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// StageClear1.cs
// �X�e�[�W1�N���A��ʂ̐�����s���N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class StageClear1 : MonoBehaviour
{
    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // �������������K�v�ȏꍇ�͂����ɒǉ�
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // X�L�[�������ꂽ��A�X�e�[�W2�ɑJ�ڂ���
        if (Input.GetKeyDown(KeyCode.X))
        {
            SceneManager.LoadScene("Stage2");
        }
    }
}