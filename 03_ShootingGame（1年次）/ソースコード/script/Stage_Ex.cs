// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Stage_Ex.cs
// �G�L�X�g���X�e�[�W�̐i�s���Ǘ�����N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Stage_Ex : MonoBehaviour
{
    // UI�ƃQ�[���I�u�W�F�N�g�ւ̎Q��
    public Text TimeText; // �o�ߎ��Ԃ�\������UI�e�L�X�g
    float gameTimer; // �o�ߎ��ԃ^�C�}�[
    public GameObject enemyPrefab; // �o��������G�̃v���n�u
    public int enemyNumber; // ��ʓ��Ɉێ�����G�̐�
    float enemyRadius; // �G�̔��a�i�X�|�[�����̏Փ˔���p�j

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        gameTimer = 0; // �^�C�}�[������

        // �Q�[���}�l�[�W���[����X�R�A��ǂݍ���ŕ\�����X�V
        ShootingGameManager mng = GameObject.Find("Main Camera").GetComponent<ShootingGameManager>();
        mng.renewScore();

        // �G�̃R���C�_�[�̔��a���擾
        enemyRadius = enemyPrefab.GetComponent<Collider2D>().bounds.extents.x;

        // ��ʓ��̓G���w�萔�ɖ����Ȃ��ꍇ�A��[����
        for (int i = GameObject.FindGameObjectsWithTag("Enemy").Length; i < enemyNumber; i = GameObject.FindGameObjectsWithTag("Enemy").Length)
        {
            // ��ʏ㕔�Ƀ����_���Ȉʒu�𐶐�
            Vector3 spawnPosition = new Vector3(Random.Range(-5.0f, 5.0f), Random.Range(2.5f, 4.5f), 10.0f);

            // �X�|�[���ʒu�ɑ��̓G�����Ȃ����`�F�b�N
            Collider2D CollisionWithEnemy = Physics2D.OverlapCircle(spawnPosition, enemyRadius, LayerMask.GetMask("EnemyLayer"));

            // �Փ˂��Ȃ���ΓG�𐶐�
            if (CollisionWithEnemy == false)
            {
                Instantiate(enemyPrefab, spawnPosition, transform.rotation);
            }
        }
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �o�ߎ��Ԃ�UI�ɕ\��
        TimeText.text = "Time Used:" + (int)gameTimer;

        // ����̓G(Enemy04)�����݂��Ȃ��ꍇ (�N���A�������H)
        if (GameObject.Find("Enemy04") == null)
        {
            // �N���A�V�[���ւ̑J�ڏ����i���݂̓R�����g�A�E�g�j
            // SceneManager.LoadScene("StageEx_Clear");
        }
        // ����̓G�����݂���ꍇ
        else if (GameObject.Find("Enemy04") != null)
        {
            // �o�ߎ��Ԃ����Z
            gameTimer += Time.deltaTime;

            // ��ʓ��̓G���w�萔��菭�Ȃ��ꍇ�A��[����
            if (GameObject.FindGameObjectsWithTag("Enemy").Length < enemyNumber)
            {
                Spawn();
            }
        }
    }

    // �G����̃X�|�[��������֐�
    void Spawn()
    {
        // ��ʏ�[�Ƀ����_���Ȉʒu�𐶐�
        Vector3 spawnPosition = new Vector3(Random.Range(-5.0f, 5.0f), 5.0f, 10.0f);

        // �X�|�[���ʒu�ɑ��̓G�����Ȃ����`�F�b�N
        Collider2D CollisionWithEnemy = Physics2D.OverlapCircle(spawnPosition, enemyRadius, LayerMask.GetMask("EnemyLayer"));

        // �Փ˂��Ȃ���ΓG�𐶐�
        if (CollisionWithEnemy == false)
        {
            Instantiate(enemyPrefab, spawnPosition, transform.rotation);
        }
    }

    // EX�X�e�[�W���I��������O������Ăяo�����߂̊֐�
    public void EndEx()
    {
        // ����ȏ�G���X�|�[�����Ȃ��悤�ɁA�ڕW����0�ɂ���
        enemyNumber = 0;
    }
}