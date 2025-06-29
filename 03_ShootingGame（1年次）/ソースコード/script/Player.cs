// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Player.cs
// �v���C���[�L�����N�^�[�̐���A����Ǘ��A��ԕω���S������N���X

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    [SerializeField] GameObject[] MainPrefab; // ���C������̃v���n�u�z��
    [SerializeField] GameObject[] SubPrefab;  // �T�u����̃v���n�u�z��
    // public GameObject doubleBulletPrefab; // ���g�p
    // public GameObject laserPrefab; // ���g�p
    // public GameObject EMPPrefab; // ���g�p
    // public GameObject BAPrefab; // ���g�p
    public int mode; // ���݂̃p���[�A�b�v���[�h (0:�ʏ�, 1:�p���[�A�b�v1, 2:�p���[�A�b�v2)
    float moveSpeed; // ���݂̈ړ����x
    public float mode0Speed; // ���[�h0�̎��̈ړ����x
    public float mode1Speed; // ���[�h1�̎��̈ړ����x
    public float mode2Speed; // ���[�h2�̎��̈ړ����x
    public GameObject explosionPrefab; // �j�󂳂ꂽ���̔����G�t�F�N�g
    public GameObject shieldPrefab; // ���G���Ԓ��̃V�[���h�G�t�F�N�g
    public GameObject bfPrefab; // �o�b�N�t�@�C�A�G�t�F�N�g
    public int MainWeaponIndex; // �I�����ꂽ���C������̃C���f�b�N�X
    public int SubWeaponIndex; // �I�����ꂽ�T�u����̃C���f�b�N�X
    private BoxCollider2D PlayerCollider; // �v���C���[�̃R���C�_�[
    public float powerUpTimer; // �p���[�A�b�v�̎c�莞��
    public float InvincibleTimer; // ���G���Ԃ̎c�莞��
    // public float fireRate; // ���g�p
    // float fireCountdown; // ���g�p
    ScoreSave sc; // �V�[���ԂŃf�[�^��ێ�����I�u�W�F�N�g

    // �v���C���[�̏��
    public enum Status
    {
        Fine,
        Dead,
    };
    
    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // ScoreSave�I�u�W�F�N�g���畐��I�������擾
        sc = GameObject.FindGameObjectWithTag("ScoreSave").GetComponent<ScoreSave>();
        mode = 0;
        MainWeaponIndex = sc.MainWeaponIndex;
        SubWeaponIndex = sc.SubWeaponIndex;
        
        // �e�^�C�}�[��������
        powerUpTimer = 0;
        InvincibleTimer = 2; // �J�n����2�b�Ԃ̖��G����

        // �V�[���h�ƃo�b�N�t�@�C�A�G�t�F�N�g�𐶐����A���g�̎q�I�u�W�F�N�g�ɂ���
        Instantiate(shieldPrefab, transform.position, Quaternion.identity).transform.parent = gameObject.transform;
        Instantiate(bfPrefab, transform.position, Quaternion.identity).transform.parent = gameObject.transform;
        
        // �I�����ꂽ����𐶐����A���g�̎q�I�u�W�F�N�g�ɂ���
        if(MainPrefab.Length > MainWeaponIndex && MainPrefab[MainWeaponIndex] != null)
            Instantiate(MainPrefab[MainWeaponIndex], transform.position, Quaternion.identity).transform.parent = gameObject.transform;
        if(SubPrefab.Length > SubWeaponIndex && SubPrefab[SubWeaponIndex] != null)
            Instantiate(SubPrefab[SubWeaponIndex], transform.position, Quaternion.identity).transform.parent = gameObject.transform;
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // ���G�^�C�}�[�����Z
        InvincibleTimer -= Time.deltaTime;
        
        // === �p���[�A�b�v��Ԃ̊Ǘ� ===
        // �{�X�����炸�A�p���[�A�b�v���Ԃ��؂ꂽ�ꍇ
        if (GameObject.FindGameObjectWithTag("Boss") == null && powerUpTimer <= 0)
		{
            mode = 0;
            // ���[�U�[�Ȃǂ̈ꕔ�G�t�F�N�g��j���i���݂̓v���C���[�ɒǏ]���邽�ߕs�v�ȉ\���j
            // Destroy(GameObject.FindGameObjectWithTag("Laser"));
        }
        // �{�X�����炸�A�p���[�A�b�v���Ԃ��c���Ă���ꍇ
        else if(GameObject.FindGameObjectWithTag("Boss") == null && powerUpTimer > 0)
        {
            powerUpTimer -= Time.deltaTime;
        }
        // �{�X������ꍇ�A�����I�ɍŋ����[�h(mode 2)�ɂ���
        else if (GameObject.FindGameObjectWithTag("Boss") != null)
        {
            mode = 2;
        }
        
        // === �ړ����� ===
        // �}�E�X�J�[�\���̃��[���h���W���擾
        Vector3 mousePosition = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        mousePosition.z = 0; // Z���W��0�ɌŒ�
        // �ړ��͈͂𐧌�
        mousePosition.x = Mathf.Clamp(mousePosition.x, -6.2f, 6.2f);
        mousePosition.y = Mathf.Clamp(mousePosition.y, -4.5f, 4.5f);
        
        // ���݈ʒu����}�E�X�J�[�\���ʒu�֊��炩�Ɉړ�
        transform.position = Vector2.Lerp(transform.position, mousePosition, moveSpeed * Time.deltaTime); // Lerp�̑�O������Time.deltaTime����Z���ăt���[�����[�g�ˑ����Ȃ���
        
        // === ���[�h�ɉ��������x�ݒ� ===
        if (mode == 0) Mode0();
        if (mode == 1) Mode1();
        if (mode >= 2) Mode2(); // 3�ȏ��2�Ƃ��Ĉ���
    }

    // ���̃R���C�_�[�ƐڐG�����u�ԂɌĂ΂�܂�
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // �G�A�G�e�A�{�X�ƐڐG���A�����G���Ԓ��łȂ��ꍇ
        if ((collision.gameObject.tag == "Enemy"|| collision.gameObject.tag == "EnemyBullet" || collision.gameObject.tag == "Boss") && InvincibleTimer <= 0)
        {
            // �v���C���[�Ɗ֘A�I�u�W�F�N�g�i���[�U�[�A�e�Ȃǁj��j��
            Destroy(gameObject);
            GameObject laser = GameObject.FindGameObjectWithTag("Laser");
            if(laser != null) Destroy(laser);

            GameObject[] main = GameObject.FindGameObjectsWithTag("PlayerBullet");
            GameObject[] sub = GameObject.FindGameObjectsWithTag("PlayerSubBullet");
            foreach (GameObject mainObj in main) Destroy(mainObj);
            foreach (GameObject subObj in sub) Destroy(subObj);
            
            // �����G�t�F�N�g�𐶐�
            Instantiate(explosionPrefab, transform.position, Quaternion.identity);
        }

        // �A�C�e���ƐڐG�����ꍇ
        if (collision.gameObject.tag == "Item")
        {
            Destroy(collision.gameObject);
            // ���[�h��2�����Ȃ�1�i�K�p���[�A�b�v
            if (mode < 2)
            {
                mode += 1;
            }
            powerUpTimer = 10; // �p���[�A�b�v���Ԃ�10�b�ɃZ�b�g
        }

        // ���C�t�A�C�e���ƐڐG�����ꍇ
        if (collision.gameObject.tag == "Life")
        {
            Destroy(collision.gameObject);
            // �Q�[���}�l�[�W���[�Ƀ��C�t�ǉ���ʒm
            ShootingGameManager mng = GameObject.Find("Main Camera").GetComponent<ShootingGameManager>();
            mng.AddLife();
        }

        // EMP�A�C�e���ƐڐG�����ꍇ
        if (collision.gameObject.tag == "EMP")
        {
            Destroy(collision.gameObject);
            // EMP�����𐶐�
            Instantiate(EMPPrefab, transform.position, Quaternion.identity);
        }
    }

    // ���[�h0�̎��̐ݒ�
    void Mode0()
	{
        moveSpeed = mode0Speed;
	}

    // ���[�h1�̎��̐ݒ�
    void Mode1()
    {
        moveSpeed = mode1Speed;
    }

    // ���[�h2�̎��̐ݒ�
    void Mode2()
    {
        moveSpeed = mode2Speed;
    }
    
    // ���݂̈ړ����x��Ԃ��֐��i���g�p�j
    public float returnSpeed()
    {
        return moveSpeed;
    }
}