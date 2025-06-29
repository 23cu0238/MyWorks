// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Boss1.cs
// �X�e�[�W1�̃{�X�̍s���p�^�[����e���𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Boss1 : MonoBehaviour
{
    public CanvasControl canvas; // UI�L�����o�X����X�N���v�g
    public float NormalMoveSpeed; // �ʏ�̈ړ����x
    Player player; // �v���C���[�I�u�W�F�N�g�ւ̎Q��
    public GameObject bulletPrefab; // ���˂���e�̃v���n�u
    public GameObject explosionPrefab; // �����G�t�F�N�g�̃v���n�u
    public GameObject BossHp; // �{�XHP�o�[��UI�I�u�W�F�N�g
    public GameObject item; // �h���b�v����A�C�e���̃v���n�u
    float bulletTimer; // �e�̔��˃^�C�~���O���Ǘ�����^�C�}�[
    public float bulletInterval; // �e�̔��ˊԊu
    ShootingGameManager mng; // �Q�[���S�̂̃}�l�[�W���[

    float hp; // ���݂�HP
    Vector2 dir; // �ړ�����

    // �{�X�̍s�����[�h���`����񋓑�
    enum Status { vert, hori } // vert: �����ړ�, hori: �����ړ�
    Status MovementMode; // ���݂̍s�����[�h

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // ����I�ɃA�C�e�����h���b�v����R���[�`�����J�n
        StartCoroutine(ItemDrop());
        
        // UI�L�����o�X���擾���A�{�XHP�o�[��L����
        canvas = GameObject.Find("Canvas").GetComponent<CanvasControl>();
        canvas.ActivateHPBar();

        // �K�v�ȃI�u�W�F�N�g��R���|�[�l���g���擾
        player = GameObject.FindGameObjectWithTag("Player").GetComponent<Player>();
        MovementMode = Status.vert; // �������[�h�͐����ړ�
        bulletTimer = 0;
        hp = GetComponent<HP>().enemyHp;
        mng = GameObject.Find("Main Camera").GetComponent<ShootingGameManager>();
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        player = GameObject.FindGameObjectWithTag("Player")?.GetComponent<Player>(); // �v���C���[�����Ȃ��ꍇ���l��

        // Y���W��2.4�ȉ��ɂȂ����琅���ړ����[�h�Ɉڍs
        if (transform.position.y <= 2.4)
        {
            MovementMode = Status.hori;
        }

        // �����ړ����[�h�̏ꍇ
        if (MovementMode == Status.vert)
        {
            // �^���Ɉړ�
            transform.Translate(0, -NormalMoveSpeed, 0);
        }
        
        // �����ړ����[�h�̏ꍇ
        if (MovementMode == Status.hori)
        {
            // �v���C���[�����݂���΁A�v���C���[��X���W��ǂ��悤�Ɉړ�
            if (player != null)
            { 
                dir = new Vector2(player.transform.position.x - transform.position.x, 0); // Y�͒ǂ�Ȃ�
            }
            transform.Translate(dir.normalized.x * NormalMoveSpeed, 0f, 0f);
        }

        // �e���˃^�C�}�[���X�V
        bulletTimer += Time.deltaTime;

        // ���ˊԊu�ɒB������e�𔭎�
        if (bulletTimer >= bulletInterval)
        {
            // �����_���Ȋp�x�i�O���������j�ɒe��3����
            for (int i = 0; i < 3; ++i)
            {
                Instantiate(bulletPrefab, transform.position, Quaternion.Euler(0, 0, Random.Range(135f, 225f)));
            }
            bulletTimer = 0.0f; // �^�C�}�[���Z�b�g
        }

        // HP���X�V
        hp = GetComponent<HP>().enemyHp;
        
        // HP��0�ȉ��ɂȂ�����
        if (hp <= 0)
        {
            // �����G�t�F�N�g�𐶐�
            Instantiate(explosionPrefab, transform.position, Quaternion.identity);
            // ���݂̃X�R�A�⃉�C�t��ۑ�
            mng.SaveInfo();
            // �X�e�[�W�N���A�������J�n
            mng.LoadClear();
            // �{�X���g��j��
            Destroy(gameObject);
        }
    }

    // ��ʊO�ɏo���玩�g��j��
    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "ActiveArea")
        {
            Destroy(gameObject);
        }
    }

    // ����I�ɃA�C�e�����h���b�v����R���[�`��
    private IEnumerator ItemDrop()
    {
        // �������[�v
        do
        {
            // 0����99�̗����𐶐�
            float rand = Random.Range(0, 99);
            // 5%�̊m���ŃA�C�e���𐶐�
            if (rand <= 5)
            {
                Instantiate(item, transform.position, Quaternion.identity);
            }
            // 1�b�ҋ@
            yield return new WaitForSeconds(1f);
        }
        while (true);
    }
}