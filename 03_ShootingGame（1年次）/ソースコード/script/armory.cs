// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// armory.cs
// ����I����ʁi�H���j��UI�ƃ��W�b�N���Ǘ�����N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using TMPro;

public class armory : MonoBehaviour
{
    // �R���|�[�l���g�ƃI�u�W�F�N�g�ւ̎Q��
    Camera MC; // ���C���J����
    Canvas canvas; // ���̃X�N���v�g���A�^�b�`����Ă���L�����o�X
    ScoreSave sc; // �V�[���ԂŃf�[�^��ێ�����ScoreSave�I�u�W�F�N�g
    public TMP_Dropdown DdMain; // ���C������I��p�̃h���b�v�_�E��
    public TMP_Dropdown DdSub; // �T�u����I��p�̃h���b�v�_�E��

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // �K�v�ȃR���|�[�l���g�ƃI�u�W�F�N�g���������Ď擾
        MC = Camera.main;
        canvas = GetComponent<Canvas>();
        canvas.worldCamera = Camera.main; // �L�����o�X�̃����_�����O�J������ݒ�
        DdMain = GameObject.Find("DropdownMain").GetComponent<TMP_Dropdown>();
        DdSub = GameObject.Find("DropdownSub").GetComponent<TMP_Dropdown>();
        sc = GameObject.FindGameObjectWithTag("ScoreSave").GetComponent<ScoreSave>();
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // ���t���[���A�h���b�v�_�E���őI�����ꂽ�������ۑ�
        SaveWeaponOption();

        // X�L�[�������ꂽ��Stage1�ɑJ��
        if (Input.GetKeyDown(KeyCode.X))
        {
            SceneManager.LoadScene("Stage1");
        }
    }

    // ����̑I������ScoreSave�I�u�W�F�N�g�ɕۑ�����֐�
    void SaveWeaponOption()
    {
        sc.MainWeaponIndex = DdMain.value; // ���C������̃C���f�b�N�X��ۑ�
        sc.SubWeaponIndex = DdSub.value;  // �T�u����̃C���f�b�N�X��ۑ�
    }
}