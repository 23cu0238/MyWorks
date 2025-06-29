// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���� 23cu0238���N�F
// PUTimer.cs
// �v���C���[�̃p���[�A�b�v�c�莞�Ԃ�UI�X���C�_�[�ŕ\������N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PUTimer : MonoBehaviour
{
    Slider PUT; // �p���[�A�b�v���ԕ\���p��UI�X���C�_�[

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        PUT = GetComponent<Slider>(); // �X���C�_�[�R���|�[�l���g���擾
        PUT.maxValue = 10; // �X���C�_�[�̍ő�l��10�b�ɐݒ�
        // �v���C���[�I�u�W�F�N�g��T���A���̃p���[�A�b�v�^�C�}�[�������l�Ƃ��Đݒ�
        GameObject playerObj = GameObject.Find("Player");
        if(playerObj != null)
        {
            PUT.value = playerObj.GetComponent<Player>().powerUpTimer;
        }
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �v���C���[�I�u�W�F�N�g��T��
        GameObject playerObj = GameObject.Find("Player");
        // �v���C���[�����Ȃ��ꍇ�i���S���Ȃǁj�̓^�C�}�[��0�ɂ���
        if (playerObj == null)
        {
            PUT.value = 0;
        }
        // �v���C���[������ꍇ�́A���݂̃p���[�A�b�v�^�C�}�[���X���C�_�[�ɔ��f
        else if (playerObj != null)
        {
            PUT.value = playerObj.GetComponent<Player>().powerUpTimer;
        }
    }
}