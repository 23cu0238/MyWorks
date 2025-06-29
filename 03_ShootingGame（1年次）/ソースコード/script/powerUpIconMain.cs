// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// powerUpIconMain.cs
// ����I����ʂŁA���C������̃h���b�v�_�E���I���ɉ����ăA�C�R����\������N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class powerUpIconMain : MonoBehaviour
{
    SpriteRenderer sr; // ���̃I�u�W�F�N�g���g��SpriteRenderer (���g�p�̉\��)
    SpriteRenderer IconSr; // �A�C�R���\���p��SpriteRenderer
    public TMP_Dropdown DdMain; // ���C������I��p�̃h���b�v�_�E��
    armory arm; // �e�I�u�W�F�N�g��armory�X�N���v�g

    [SerializeField] Sprite[] sprite; // �h���b�v�_�E���̑I�����ɑΉ�����X�v���C�g�̔z��

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        sr = GetComponent<SpriteRenderer>();
        // �V�[������"IconMain"�I�u�W�F�N�g����SpriteRenderer���擾
        IconSr = GameObject.Find("IconMain").GetComponent<SpriteRenderer>();
        // �q�I�u�W�F�N�g����h���b�v�_�E���R���|�[�l���g���擾
        DdMain = GetComponentInChildren<TMP_Dropdown>();
        // �e�I�u�W�F�N�g����armory�R���|�[�l���g���擾
        arm = GetComponentInParent<armory>();
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �h���b�v�_�E���̌��݂̑I��l(value)���C���f�b�N�X�Ƃ��āA�Ή�����X�v���C�g���A�C�R���ɐݒ�
        if (IconSr != null && DdMain != null && sprite.Length > DdMain.value)
        {
            IconSr.sprite = sprite[DdMain.value];
        }
    }
}