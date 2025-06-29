// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// ShootingGameManager.cs
// �Q�[���S�̂̐i�s�A�X�R�A�A�c�@�A�v���C���[�̃X�|�[�����Ǘ�����}�l�[�W���[�N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class ShootingGameManager : MonoBehaviour
{
    // �����o�ϐ�
    float gameOverTime = 3; // �v���C���[�̎c�@��0�̎��ɃQ�[���I�[�o�[�ɂȂ�܂ł̎���
    float playerDeadTimer; // �v���C���[������Ă���̌o�ߎ��Ԃ��L�^����^�C�}�[
    float respawnTime = 2; // �v���C���[������Ă��烊�X�|�[������܂ł̑ҋ@����
    float deadTimer; // �v���C���[�����Ȃ���Ԃ������Ă���̌o�ߎ��ԃ^�C�}�[
    Player.Status playerStatus; // �v���C���[�̏�Ԃ��Ǘ� (Fine:����, Dead:���S)
    public Text ScoreText; // �X�R�A��\������UI�e�L�X�g
    public Text LifeText; // �c�@��\������UI�e�L�X�g
    public int totalScore; // ���݂̍��v�X�R�A
    public int Life; // ���݂̎c�@
    public GameObject PlayerPrefab; // ���X�|�[��������v���C���[�̃v���n�u
    static Vector3 spawnPosition; // �v���C���[�̃��X�|�[���ʒu
    float endTimer; // �X�e�[�W�N���A���̉��o�p�^�C�}�[
    bool IsEnd; // �X�e�[�W�N���A���o�����ǂ����̃t���O

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // �e�^�C�}�[�ƃt���O��������
        endTimer = 0;
        playerDeadTimer = 0;
        playerStatus = Player.Status.Fine;

        // �V�[�����܂����Ńf�[�^��ێ�����ScoreSave�I�u�W�F�N�g��T��
        if (GameObject.FindGameObjectWithTag("ScoreSave") != null)
        {
            // ScoreSave�I�u�W�F�N�g����X�R�A�Ǝc�@��ǂݍ���
            ScoreSave ss = GameObject.FindGameObjectWithTag("ScoreSave").GetComponent<ScoreSave>();
            totalScore = ss.GetSavedScore();
            Life = ss.GetSavedLife();
        }

        // UI�e�L�X�g��������
        ScoreText.text = "Total Score:" + totalScore;
        LifeText.text = "Life left:"+Life;
        // ���X�|�[���ʒu��ݒ�
        spawnPosition = new Vector3(0.0f, -3.5f, 0.0f);
        deadTimer = 0;
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �X�e�[�W�N���A���o���̏���
        if (IsEnd==true)
        {
            endTimer += Time.deltaTime; // �^�C�}�[��i�߂�
            // 2�b�o�߂�����A���݂̃X�e�[�W���ɉ����Ď��̃V�[���ɑJ��
            if(endTimer>=2)
            {
                if (SceneManager.GetActiveScene().name == "Stage1")
                {
                    SceneManager.LoadScene("Stage1_Clear");
                }
                else if (SceneManager.GetActiveScene().name == "Stage2")
                {
                    SceneManager.LoadScene("Stage2_Clear");
                }
                else if (SceneManager.GetActiveScene().name == "Stage3")
                {
                    SceneManager.LoadScene("Stage3_ExClear");
                }
            }    
        }    

        // �v���C���[�����S�Ɏ��S������ԁi�c�@0�Ŕj�󂳂ꂽ��j�̏���
        if (playerStatus==Player.Status.Dead)
        {
            playerDeadTimer += Time.deltaTime; // �^�C�}�[��i�߂�
            
            // �Q�[���I�[�o�[�ҋ@���Ԃ𒴂�����Q�[���I�[�o�[�V�[���ɑJ��
            if(playerDeadTimer>= gameOverTime)
            {
                SceneManager.LoadScene("GameOver");
                playerDeadTimer = 0.0f; // �^�C�}�[���Z�b�g
            }
        }

        // �v���C���[�I�u�W�F�N�g�����݂����A���c�@��0�̏ꍇ
        if (GameObject.FindGameObjectsWithTag("Player").Length < 1 && Life <= 0)
        {
            // �v���C���[�̏�Ԃ��u���S�v�ɐݒ�
            playerStatus = Player.Status.Dead;
        }
        // �v���C���[�I�u�W�F�N�g�͑��݂��Ȃ����A�c�@���܂�����ꍇ
        else if (GameObject.FindGameObjectsWithTag("Player").Length < 1 && Life > 0)
        {
            // ���X�|�[���ҋ@�^�C�}�[��i�߂�
            deadTimer += Time.deltaTime;
        }

        // ���X�|�[���ҋ@���Ԃ𒴂�����
        if( deadTimer >= respawnTime)
        {
            // �v���C���[�����X�|�[�������A�c�@��1���炷
            Instantiate(PlayerPrefab, spawnPosition, Quaternion.identity);
            Life--;
            deadTimer = 0; // �^�C�}�[���Z�b�g
        }

        // �c�@�\�����X�V
        LifeText.text = "Life left:" + Life;
    }

   
    // �X�R�A�����Z����֐�
    public void AddScore(int score)
    {
        totalScore += score;
        ScoreText.text = "Total Score:"+totalScore.ToString(); // UI���X�V
    }

    // ���݂̃X�R�A���擾����֐�
    public int GetScore()
    {
        return totalScore;
    }

    // ���݂̏���ScoreSave�I�u�W�F�N�g�ɕۑ�����֐�
    public void SaveInfo()
    {
        ScoreSave sc=GameObject.FindGameObjectWithTag("ScoreSave").GetComponent<ScoreSave>();
        sc.SaveScore(totalScore);
        sc.SaveLife(Life);
    }

    // �X�R�A�\���������I�ɍX�V����֐�
    public void renewScore()
    {
        ScoreText.text = "Total Score:" + totalScore.ToString();
    }
    
    // �c�@�����Z�b�g����֐�
    public void resetLife()
    {
        Life = 3;
    }

    // �c�@��1���₷�֐�
    public void AddLife()
    {
        Life+=1;
    }

    // �X�e�[�W�N���A���o���J�n���邽�߂̊֐�
    public void LoadClear()
    {
        IsEnd = true;
    }
}