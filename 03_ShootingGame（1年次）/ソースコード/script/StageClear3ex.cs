// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// StageClear3ex.cs
// �X�e�[�W3�N���A��AEX�X�e�[�W�֑J�ڂ��邽�߂̉�ʂ𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class StageClear3ex : MonoBehaviour
{
    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // X�L�[�������ꂽ��EX�X�e�[�W�ɑJ�ڂ���
        if (Input.GetKeyDown(KeyCode.X))
        {
            SceneManager.LoadScene("StageEx");
        }
    }
}