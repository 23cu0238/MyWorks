// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// HomingCluster.cs
// �v���C���[��ǔ�����N���X�^�[�~�T�C���̋����𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HomingCluster : MonoBehaviour
{
    public float moveSpeed; // �ړ����x
    public float power; // �U����
    GameObject[] Enemy; // �V�[�����̓G�I�u�W�F�N�g�z��
    GameObject Boss; // �V�[�����̃{�X�I�u�W�F�N�g
    GameObject player; // �v���C���[�I�u�W�F�N�g
    public GameObject explosion; // �����G�t�F�N�g�̃v���n�u
    public GameObject NearestEnemy; // �ł��߂��G
    Rigidbody2D rigidBody; // Rigidbody2D�R���|�[�l���g
    public float angleChangingSpeed; // �ǔ����̐��񑬓x
    
    // �~�T�C���̏��
    public enum Mode
    {
        initial, // �����g�U�t�F�[�Y
        chasing, // �ǔ��t�F�[�Y
    };

    public Mode mode; // ���݂̏��
    float timer; // �o�ߎ��ԃ^�C�}�[

    public Vector3 startPos; // �����g�U���̖ڕW�ʒu
    Vector2 tempPos; // �v���C���[�������������̖ڕW�ʒu
   
    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player");
        mode = Mode.initial; // �������[�h�ݒ�
        // ���˒n�_�̎��͂Ƀ����_���ȖڕW�n�_��ݒ�
        Vector2 randPos = Random.insideUnitCircle * 2;
        startPos = new Vector3(randPos.x + transform.position.x, (-Mathf.Abs(randPos.y)) + transform.position.y, 0);
        timer = 0;
        rigidBody = GetComponent<Rigidbody2D>();
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        player = GameObject.FindGameObjectWithTag("Player");
        if (player != null)
        {
            tempPos = player.transform.position; // �v���C���[�̈ʒu��ۑ�
        }

        timer += Time.deltaTime; // �^�C�}�[�X�V

        // === �ł��߂��G��T�� ===
        Vector3 pos = transform.position;
        float dist = float.PositiveInfinity;
        Enemy = GameObject.FindGameObjectsWithTag("Enemy");
        Boss = GameObject.FindGameObjectWithTag("Boss");
        // �ʏ�̓G����ł��߂����̂�T��
        foreach (var obj in Enemy)
        {
            var d = (pos - obj.transform.position).sqrMagnitude;
            if (d < dist)
            {
                NearestEnemy = obj;
                dist = d;
            }
        }
        // �{�X�����āA���ʏ�̓G���߂��ꍇ�̓{�X���^�[�Q�b�g�ɂ���
        if (Boss != null)
        {
            var bossDis = (pos - Boss.transform.position).sqrMagnitude;
            if (bossDis < dist)
            {
                NearestEnemy = Boss;
            }
        }

        // === �ړ����� ===
        // �^�[�Q�b�g�i�ł��߂��G�j������ꍇ
        if (NearestEnemy != null)
        {
            // �����g�U�t�F�[�Y
            if (mode == Mode.initial)
            {
                // �����_���ȖڕW�n�_�Ɍ������Ē��i
                Vector2 dir = (Vector2)startPos - (Vector2)transform.position;
                transform.Translate(dir.normalized * 2 * Time.deltaTime, Space.World); // ���[���h���W�n�ňړ�
            }
            // �ǔ��t�F�[�Y
            else
            {
                // �^�[�Q�b�g�Ɍ������Đ��񂵂Ȃ���ǔ�
                Vector2 direction = (Vector2)NearestEnemy.transform.position - rigidBody.position;
                direction.Normalize();
                float rotateAmount = Vector3.Cross(direction, transform.up).z;
                rigidBody.angularVelocity = -angleChangingSpeed * rotateAmount;
                rigidBody.velocity = transform.up * moveSpeed;
            }
        }
        // �^�[�Q�b�g�����Ȃ��ꍇ
        else
        {
            // �Ō�Ɋm�F�����v���C���[�̈ʒu�Ɍ������Ēǔ�
            Vector2 direction = tempPos - rigidBody.position;
            direction.Normalize();
            float rotateAmount = Vector3.Cross(direction, transform.up).z;
            rigidBody.angularVelocity = -angleChangingSpeed * rotateAmount;
            rigidBody.velocity = transform.up * moveSpeed;
        }

        // 1�b��ɒǔ��t�F�[�Y�Ɉڍs
        if (timer >= 1)
        {
            mode = Mode.chasing;
        }
        // 10�b��Ɏ���
        if (timer >= 10)
        {
            if (explosion != null)
            {
                Instantiate(explosion, transform.position, Quaternion.identity);
            }
            Destroy(gameObject);
        }
    }

    // ���̃R���C�_�[�ƐڐG�����u�ԂɌĂ΂�܂�
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Enemy" || collision.gameObject.tag == "Boss")
        {
            // ����Ƀ_���[�W��^���Ď��g�͏���
            HP hit = collision.gameObject.GetComponent<HP>();
            if (hit != null)
            {
                hit.Damage(power);
            }
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
}