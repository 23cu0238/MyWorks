// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// GameOver.cs
// �Q�[���I�[�o�[��ʂōŏI�X�R�A�\���ƃ^�C�g�����A�𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class GameOver : MonoBehaviour
{
    int finalScore; // �ŏI�X�R�A
    public TextMeshProUGUI ScoreText; // �X�R�A�\���p��UI�e�L�X�g

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // �V�[�����܂����Ńf�[�^��ێ����Ă���ScoreSave�I�u�W�F�N�g��T��
        GameObject scoreSaveObject = GameObject.FindGameObjectWithTag("ScoreSave");
        if (scoreSaveObject != null)
        {
            // ScoreSave�R���|�[�l���g����ۑ�����Ă���X�R�A���擾
            ScoreSave ss = scoreSaveObject.GetComponent<ScoreSave>();
            finalScore = ss.GetSavedScore();
        }
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // UI�e�L�X�g�ɍŏI�X�R�A�Ƒ���ē���\��
        ScoreText.text = "Total Score:" + finalScore.ToString() + "\nPress 'X' to continue." ;

        // X�L�[�������ꂽ��^�C�g���V�[���ɑJ��
        if (Input.GetKeyDown(KeyCode.X))
        {
            SceneManager.LoadScene("Title");
        }
    }
}