// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// GameClear.cs
// �Q�[���N���A��ʂŃ^�C�g���ւ̕��A�𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameClear : MonoBehaviour
{
    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {

    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // X�L�[�������ꂽ��^�C�g���V�[���ɑJ��
        if (Input.GetKeyDown(KeyCode.X))
        {
            SceneManager.LoadScene("Title");
        }
    }
}