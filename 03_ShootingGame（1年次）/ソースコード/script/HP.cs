// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// HP.cs
// �G��I�u�W�F�N�g�̗̑́iHP�j���Ǘ�����ėp�R���|�[�l���g

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HP : MonoBehaviour
{
    public float enemyHp; // HP�̌��ݒl
    private bool DropIsEnabled; // �A�C�e���h���b�v���L�����ǂ����̃t���O
    public GameObject explosionPrefab; // �ʏ�̔����G�t�F�N�g�̃v���n�u
    public GameObject itemPrefab; // �p���[�A�b�v�A�C�e���̃v���n�u
    public GameObject LifePrefab; // ���C�t�A�b�v�A�C�e���̃v���n�u
    public GameObject EMPPrefab; // EMP�A�C�e���̃v���n�u
    public GameObject EMPExplosion; // �{�X���j����EMP�����G�t�F�N�g
    public int scoreEarned; // ���̓G��|�����Ƃ��ɓ�����X�R�A
    public float itemDropRate; // �p���[�A�b�v�A�C�e���̃h���b�v��
    public float lifeDropRate; // ���C�t�A�b�v�A�C�e���̃h���b�v��
    public float EMPDropRate; // EMP�A�C�e���̃h���b�v��
    float DamageTimer; // �_���[�W���󂯂���̐F�ύX�^�C�}�[
    bool IsDamaged; // �_���[�W���󂯂Ă���Œ����ǂ����̃t���O
    bool IsEnd; // �{�X�����j�V�[�P���X�����ǂ����̃t���O
    private int n; // �h���b�v����p�̗���
    SpriteRenderer sr; // �X�v���C�g�`��p�R���|�[�l���g
    ProcessManager pm; // �X�e�[�W�i�s�x�}�l�[�W���[
    public int ProcessValue; // ���̓G��|�����Ƃ��̐i�s�x�㏸�l

    public float LifeDropRate { get => lifeDropRate; set => lifeDropRate = value; } // �v���p�e�B�i���ݖ��g�p�j

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        sr = GetComponent<SpriteRenderer>();
        DropIsEnabled = true;
        DamageTimer = 0;
        IsDamaged = false;
        IsEnd = false;
        pm = GameObject.Find("Main Camera")?.GetComponent<ProcessManager>(); // ?.��null�`�F�b�N���ȗ���
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // === HP��0�ȉ��ɂȂ������̏��� ===
        // �{�X("Orga")�����j����A�܂��I���V�[�P���X�ɓ����Ă��Ȃ��ꍇ
        if (enemyHp <= 0 && gameObject.name == "Orga" && IsEnd == false)
        {
            enemyHp = 0; // HP���}�C�i�X�ɂȂ�Ȃ��悤��
            BossEnd(); // �{�X���j�V�[�P���X���J�n
            IsEnd = true;
        }
        // �ʏ�̓G("Enemy"�^�O)�����j���ꂽ�ꍇ
        else if(enemyHp <= 0 && gameObject.tag == "Enemy")
        {
            explosion(); // �ʏ�̔�������
        }
        
        // === �_���[�W���󂯂����̐F�ύX���� ===
        if (IsDamaged == true)
        {
            // �X�v���C�g��Ԃ݂��������F�ɂ���
            sr.color = new Color(1f, 0.46f, 0.52f, 1f);
            DamageTimer += Time.deltaTime;

            // 0.08�b��Ɍ��̐F�ɖ߂�
            if (DamageTimer > 0.08)
            {
                sr.color = Color.white;
                IsDamaged = false;
                DamageTimer = 0;
            }
        }
    }

    // �_���[�W���󂯂�֐�
    public void Damage(float damage)
    {
        enemyHp -= damage;
    }

    // �ʏ�̓G���j�󂳂ꂽ�Ƃ��̏���
    public void explosion()
    {
        // �i�s�x���v���X
        if (pm != null)
        {
            pm.ProcessPlus(ProcessValue);
        }
        
        // �����G�t�F�N�g�𐶐�
        Instantiate(explosionPrefab, transform.position, Quaternion.identity);

        // �X�R�A�����Z
        ShootingGameManager mng = GameObject.Find("Main Camera")?.GetComponent<ShootingGameManager>();
        if (mng != null)
        {
            mng.AddScore(scoreEarned);
        }
        
        // === �A�C�e���h���b�v���� ===
        if (DropIsEnabled == true)
        {
            n = Random.Range(1, 101); // 1����100�܂ł̗���
            // �e�A�C�e���̃h���b�v���Ɋ�Â��Ĕ��肵�A����
            if (n <= itemDropRate)
            {
                Instantiate(itemPrefab, transform.position, Quaternion.identity);
            }
            if (n <= LifeDropRate)
            {
                Instantiate(LifePrefab, transform.position, Quaternion.identity);
            }
            if (n <= EMPDropRate)
            {
                Instantiate(EMPPrefab, transform.position, Quaternion.identity);
            }
        }

        // �Ō�Ɏ��g��j��
        Destroy(gameObject);
    }

    // ���̃R���C�_�[�ƐڐG�����u�ԂɌĂ΂�܂�
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // EMP�����ɓ��������ꍇ�A�A�C�e�����h���b�v���Ȃ��悤�ɂ���
        if (collision.gameObject.tag == "EMPExplosion")
        {
            DropIsEnabled = false;
        }
        // �v���C���[�̒e�ɓ��������ꍇ�A�_���[�W�F�ύX�t���O�𗧂Ă�
        if (collision.gameObject.tag == "PlayerBullet" || collision.gameObject.tag == "PlayerSubBullet")
        {
            IsDamaged = true;
            DamageTimer = 0; // �^�C�}�[���Z�b�g
        }
    }

    // ���̃R���C�_�[�ƐڐG���Ă���ԁA���t���[���Ă΂�܂�
    private void OnTriggerStay2D(Collider2D collision)
    {
        // �v���C���[�̃��[�U�[�ɓ������Ă���ԁA�_���[�W�F�ύX�t���O�𗧂đ�����
        if (collision.gameObject.name == "Laser(clone)") // ���O�ł̔�r�͕s����Ȃ��߁A�^�O�⃌�C���[�̎g�p�𐄏�
        {
            IsDamaged = true;
            DamageTimer = 0;
        }
    }

    // �{�X�����j���ꂽ�Ƃ��̐�p����
    public void BossEnd()
    {
        Instantiate(EMPExplosion, transform.position, Quaternion.identity);
        Stage_Ex ex =  GameObject.Find("Main Camera")?.GetComponent<Stage_Ex>();
        if(ex != null) ex.EndEx();
        
        Boss bs = GetComponent<Boss>();
        if(bs != null) bs.EndEx();
        
        Player pl = GameObject.Find("Player")?.GetComponent<Player>();
        if(pl != null) pl.InvincibleTimer = 999; // �v���C���[�𖳓G�ɂ���
    }
}