// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// HPCanvas.cs
// �G��HP�o�[�Ȃǂ�\�����郏�[���h���UI�L�����o�X�̃J�����ݒ���s���N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HPCanvas : MonoBehaviour
{
    Canvas canvas; // ���̃X�N���v�g���A�^�b�`����Ă���Canvas�R���|�[�l���g

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        canvas = GetComponent<Canvas>();
        
        // �L�����o�X�̃����_�����O���[�h�� "World Space" �̏ꍇ�A
        // �C�x���g���󂯎�邽�߂̃J�����iEvent Camera�j��ݒ肷��K�v������
        if(canvas != null && canvas.renderMode == RenderMode.WorldSpace)
        {
            // �V�[���̃��C���J�������C�x���g�J�����Ƃ��Đݒ�
            canvas.worldCamera = Camera.main;
        }
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �X�V�����͓��ɖ���
    }
}