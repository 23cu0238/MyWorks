// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Stage3.cs
// �X�e�[�W3�̓G�̏o���p�^�[����i�s���Ǘ�����N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Stage3 : MonoBehaviour
{
    // UI�ƃQ�[���I�u�W�F�N�g�ւ̎Q��
    public Text TimeText; // ���݂͖��g�p
    public float timeLimit; // ���݂͖��g�p
    public GameObject enemyPrefab; // �G�^�C�v1
    public GameObject enemy2Prefab; // �G�^�C�v2
    public GameObject enemy3Prefab; // �G�^�C�v3
    public GameObject BossPrefab; // �{�X
    public GameObject CautionPrefab; // �x���T�C��
    public int enemyNumber; // (���݂͖��g�p)
    
    // �X�e�[�W�i�s�Ǘ��p�̕ϐ�
    ProcessManager pm;
    bool BossSpawned;
    public GameObject boss;

    // �X�e�[�W�̏��
    public enum Status
    {
        normal,
        boss,
    };
    Status status;

    // �G�̏o���ʒu
    GameObject[] SP;

    // �G�̏o���E�F�[�u�Ǘ�
    public int wave;
    float spawnTimer;

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // �R���|�[�l���g���擾
        pm = GetComponent<ProcessManager>();
        
        // ������Ԃ�ݒ�
        status = Status.normal;
        wave = 1;
        spawnTimer = 0;
        BossSpawned = false;

        // �G�̏o���ʒu(Spawn Point)�𖼑O�Ō������Ĕz��Ɋi�[
        {
            SP = new GameObject[19];
            for (int i = 0; i <= 9; i++) { SP[i] = GameObject.Find("SP" + i); }
            for (int i = 1; i <= 9; i++) { SP[9 + i] = GameObject.Find("SP-" + i); }
        }
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �i�s�x���^�[�Q�b�g�ɒB������{�X��Ɉڍs
        if (pm.process >= pm.target)
        {
            status = Status.boss;
        }

        // �ʏ��Ԃ̏���
        if (status == Status.normal)
        {
            spawnTimer -= Time.deltaTime;

            // �^�C�}�[��0�ȉ��ɂȂ����玟�̃E�F�[�u���J�n
            if (spawnTimer <= 0)
            {
                // �E�F�[�u���ɉ����ĈقȂ�G�̑g�ݍ��킹���X�|�[��
                switch (wave)
                {
                    case 1:
                        StartCoroutine(SpActivate(SP[0], enemy2Prefab, 10));
                        StartCoroutine(SpActivate(SP[2], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[11], enemyPrefab, 10));
                        spawnTimer = 5;
                        wave++;
                        break;

                    case 2:
                        StartCoroutine(SpActivate(SP[1], enemy2Prefab, 10));
                        StartCoroutine(SpActivate(SP[10], enemy3Prefab, 10));
                        StartCoroutine(SpActivate(SP[5], enemy2Prefab, 10));
                        StartCoroutine(SpActivate(SP[14], enemy3Prefab, 10));
                        spawnTimer = 5;
                        wave++;
                        break;

                    case 3:
                        StartCoroutine(SpActivate(SP[8], enemy2Prefab, 15));
                        StartCoroutine(SpActivate(SP[17], enemy2Prefab, 15));
                        spawnTimer = 5;
                        wave++;
                        break;

                    case 4:
                        StartCoroutine(SpActivate(SP[3], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[4], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[6], enemy3Prefab, 6));
                        StartCoroutine(SpActivate(SP[12], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[13], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[15], enemy3Prefab, 6));
                        spawnTimer = 5;
                        wave++;
                        break;

                    case 5:
                        StartCoroutine(SpActivate(SP[7], enemy2Prefab, 10));
                        StartCoroutine(SpActivate(SP[9], enemy3Prefab, 10));
                        StartCoroutine(SpActivate(SP[16], enemy2Prefab, 10));
                        StartCoroutine(SpActivate(SP[18], enemy3Prefab, 10));
                        spawnTimer = 5;
                        wave = 1; // �Ō�̃E�F�[�u���I�������ŏ��ɖ߂�
                        break;
                        
                    default:
                        wave = 1;
                        break;
                }
            }
        }
        // �{�X���Ԃ̏���
        else if (status == Status.boss)
        {
            // �c���Ă���G���G��S�Ĕj��
            GameObject[] EnemyLeft = GameObject.FindGameObjectsWithTag("Enemy");
            foreach (var obj in EnemyLeft)
            {
                Destroy(obj.gameObject);
            }
            
            // �{�X���܂��o�����Ă��Ȃ���΁A�o���������J�n
            if (BossSpawned == false)
            {
                StartCoroutine(SummonBoss());
                BossSpawned = true;
            }
        }
    }

    // �{�X����������R���[�`��
    private IEnumerator SummonBoss()
    {
        Instantiate(CautionPrefab, new Vector3(0, 0, 0), Quaternion.identity);
        yield return new WaitForSeconds(2f);
        Instantiate(BossPrefab, SP[0].transform.position, Quaternion.identity);
        yield return new WaitForSeconds(2f);
        BossSpawned = true;
    }

    // �w�肳�ꂽ�X�|�[���|�C���g����G�����I�ɏo��������ėp�R���[�`��
    private IEnumerator SpActivate(GameObject spawnPoint, GameObject enemy, float interval)
    {
        do
        {
            if (spawnPoint)
            {
                Instantiate(enemy, spawnPoint.transform.position, Quaternion.identity);
            }
            yield return new WaitForSeconds(interval);
        } while (status == Status.normal);
    }
}