// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// control.cs
// �^�C�g����ʂȂǂő�������̕\���𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class control : MonoBehaviour
{
    public GameObject Canvas; // �\��/��\����؂�ւ���Canvas�I�u�W�F�N�g
    Camera MC; // ���C���J����

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        MC = Camera.main; 
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // X�L�[�������ꂽ��
        if (Input.GetKeyDown(KeyCode.X))
        {
            // Canvas���A�N�e�B�u�ɂ��A���̃I�u�W�F�N�g���g��j������
            if (Canvas != null)
            {
                Canvas.SetActive(true);
            }
            Destroy(gameObject);
        }
    }
}