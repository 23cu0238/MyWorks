// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// StageClear3.cs
// �X�e�[�W3�N���A��ʂ̐�����s���N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class StageClear3 : MonoBehaviour
{
    // �����o�ϐ�
    int finalScore; // �ŏI�X�R�A���i�[����ϐ�
    public TextMeshProUGUI ScoreText; // �X�R�A��\������UI�e�L�X�g

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // ScoreSave�I�u�W�F�N�g����ŏI�X�R�A���擾
        ScoreSave ss = GameObject.FindGameObjectWithTag("ScoreSave").GetComponent<ScoreSave>();
        finalScore = ss.GetSavedScore();
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �X�R�A��UI�e�L�X�g�ɕ\��
        ScoreText.text = "Total Score:" + finalScore.ToString();

        // X�L�[�������ꂽ��^�C�g����ʂɖ߂�
        if (Input.GetKeyDown(KeyCode.X))
        {
            SceneManager.LoadScene("Title");
        }
    }
}