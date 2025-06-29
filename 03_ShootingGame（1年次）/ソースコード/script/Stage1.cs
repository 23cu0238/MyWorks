// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Stage1.cs
// �X�e�[�W1�̓G�̏o���p�^�[����i�s���Ǘ�����N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Stage1 : MonoBehaviour
{
    // UI�ƃQ�[���I�u�W�F�N�g�ւ̎Q��
    public Text TimeText; // ���݂͖��g�p�ł����A���ԕ\���p��UI�e�L�X�g
    public float timeLimit; // ���݂͖��g�p�ł����A�������ԗp�̕ϐ�
    public GameObject enemyPrefab; // �o��������G�̃v���n�u
    public GameObject BossPrefab; // �o��������{�X�̃v���n�u
    public GameObject CautionPrefab; // �{�X�o���O�ɕ\������x���T�C���̃v���n�u
    
    // �X�e�[�W�i�s�Ǘ��p�̕ϐ�
    ShootingGameManager mng; // �Q�[���}�l�[�W���[
    ProcessManager pm; // �i�s�x�}�l�[�W���[
    bool BossSpawned; // �{�X�����ɏo���������ǂ����̃t���O
    public GameObject boss; // �o�������{�X�I�u�W�F�N�g�ւ̎Q��

    // �X�e�[�W�̏�Ԃ��Ǘ�����enum
    public enum Status
    {
        normal, // �ʏ�̓G���o��������
        boss,   // �{�X��̏��
    };
    Status status;

    // �G�̏o���ʒu��ێ�����z��
    GameObject[] SP;

    // �G�̏o���E�F�[�u�Ǘ�
    public int wave; // ���݂̃E�F�[�u��
    float spawnTimer; // ���̃E�F�[�u�܂ł̃^�C�}�[
    float endTimer; // (���݂͖��g�p)

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // �R���|�[�l���g���擾
        pm = GetComponent<ProcessManager>();
        mng = GameObject.Find("Main Camera").GetComponent<ShootingGameManager>();
        
        // ������Ԃ�ݒ�
        status = Status.normal;
        wave = 1;
        spawnTimer = 0;
        BossSpawned = false;

        // �G�̏o���ʒu(Spawn Point)�𖼑O�Ō������Ĕz��Ɋi�[
        {
            SP = new GameObject[19];
            for (int i = 0; i <= 9; i++)
            {
                SP[i] = GameObject.Find("SP" + i);
            }
            for (int i = 1; i <= 9; i++)
            {
                SP[9 + i] = GameObject.Find("SP-" + i);
            }
        }
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �i�s�x���^�[�Q�b�g�ɒB������{�X��Ɉڍs
        if (pm.process>=pm.target)
        {
            status = Status.boss;
        }

        // �ʏ��Ԃ̏���
        if (status == Status.normal)
        {
            spawnTimer -= Time.deltaTime; // �X�|�[���^�C�}�[�����炷

            // �^�C�}�[��0�ȉ��ɂȂ����玟�̃E�F�[�u���J�n
            if (spawnTimer <= 0)
            {
                // ���݂̃E�F�[�u���ɉ����ēG�̏o���p�^�[��������
                switch (wave)
                {
                    case 1:
                        StartCoroutine(SpActivate(SP[0], enemyPrefab, 10)); // ���̂�ėp��
                        StartCoroutine(SpActivate(SP[2], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[11], enemyPrefab, 10));
                        spawnTimer = 5;
                        wave++;
                        break;
                    case 2:
                        StartCoroutine(SpActivate(SP[1], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[10], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[5], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[14], enemyPrefab, 10));
                        spawnTimer = 5;
                        wave++;
                        break;
                    // ...���̃E�F�[�u�����l...
                    case 3:
                        StartCoroutine(SpActivate(SP[8], enemyPrefab, 15));
                        StartCoroutine(SpActivate(SP[17], enemyPrefab, 15));
                        spawnTimer = 5;
                        wave++;
                        break;
                    case 4:
                        StartCoroutine(SpActivate(SP[3], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[4], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[6], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[12], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[13], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[15], enemyPrefab, 6));
                        spawnTimer = 5;
                        wave++;
                        break;
                    case 5:
                        StartCoroutine(SpActivate(SP[7], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[9], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[16], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[18], enemyPrefab, 10));
                        spawnTimer = 5;
                        wave++; // ����ȏ�E�F�[�u���Ȃ��ꍇ�A���[�v�𑱂��邩����
                        break;
                    default:
                        // �S�ẴE�F�[�u���I��������A�^�C�}�[�����Z�b�g���ă��[�v
                        wave = 1;
                        spawnTimer = 10;
                        break;
                }
            }
        }
        // �{�X���Ԃ̏���
        else if(status==Status.boss)
        {
            // �c���Ă���G���G��S�Ĕj��
            GameObject[] EnemyLeft = GameObject.FindGameObjectsWithTag("Enemy");
            foreach(var obj in EnemyLeft)
            {
                Destroy(obj.gameObject);
            }
            
            // �{�X���܂��o�����Ă��Ȃ���΁A�o���������J�n
            if (BossSpawned==false)
            {
                StartCoroutine(SummonBoss());
                BossSpawned = true;
            }
        }
    }

    // �{�X����������R���[�`��
    private IEnumerator SummonBoss()
    {
        // �x���T�C����\��
        Instantiate(CautionPrefab, new Vector3(0,0,0), Quaternion.identity);
        // 2�b�ҋ@
        yield return new WaitForSeconds(2f);
        // �{�X���o��������
        Instantiate(BossPrefab, SP[0].transform.position, Quaternion.identity);
        yield return new WaitForSeconds(2f); // �{�X�o����̑ҋ@�i�C�Ӂj
        BossSpawned = true;
    }

    // �w�肳�ꂽ�X�|�[���|�C���g����G�����I�ɏo��������ėp�R���[�`��
    private IEnumerator SpActivate(GameObject spawnPoint, GameObject enemy, float interval)
    {
        // �ʏ��Ԃł������A�������J��Ԃ�
        do
        {
            Instantiate(enemy, spawnPoint.transform.position, Quaternion.identity);
            yield return new WaitForSeconds(interval);
        } while (status == Status.normal);
    }
}