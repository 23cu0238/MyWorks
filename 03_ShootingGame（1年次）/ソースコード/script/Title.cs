// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Title.cs
// �^�C�g����ʂ̐�����s���N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Title : MonoBehaviour
{
    // UI�ƃQ�[���I�u�W�F�N�g�ւ̎Q��
    public GameObject Canvas; // ��������Ȃǂ�\������L�����o�X
    public GameObject BG; // �w�i�I�u�W�F�N�g (���݂͖��g�p)
    private bool showInstruction; // ����������\������Ă��邩�ǂ����̃t���O
    public GameObject ScPrefab; // ScoreSave�I�u�W�F�N�g�̃v���n�u
    public GameObject armory; // ����I����ʂ̃I�u�W�F�N�g (���݂͖��g�p)

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // �V�[����ScoreSave�I�u�W�F�N�g�����݂��Ȃ��ꍇ�A�v���n�u���琶������
        if (GameObject.FindGameObjectWithTag("ScoreSave") == null)
		{
            Instantiate(ScPrefab);
		}
        
        // ScoreSave�I�u�W�F�N�g���擾���A�X�R�A�Ǝc�@�����Z�b�g
        ScoreSave ss = GameObject.FindGameObjectWithTag("ScoreSave").GetComponent<ScoreSave>();
        ss.clrScore();
        ss.resetLife();
        
        // ��������̕\���t���O��������
        showInstruction = false;
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // X�L�[�������ꂽ�ꍇ
        if (Input.GetKeyDown(KeyCode.X))
        {
            // ����������܂��\������Ă��Ȃ����
            if (showInstruction == false)
            {
                // �L�����o�X���A�N�e�B�u�ɂ��đ��������\��
                Canvas.SetActive(true);
                showInstruction = true;
            }
        }
    }
}