// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// enemy.cs
// �ʏ�̓G�L�����N�^�[�̊�{�I�ȓ����ƍU���𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class enemy : MonoBehaviour
{
    public float NormalMoveSpeed; // ��{�̈ړ����x
    float HoriMoveSpeed; // ���������̈ړ����x
    public GameObject bulletPrefab; // ���˂���e�̃v���n�u
    float bulletTimer; // �e�̔��˃^�C�~���O���Ǘ�����^�C�}�[
    public float bulletInterval; // �e�̔��ˊԊu
    
    // �G�̍s�����[�h���`����񋓑�
    enum Status { vert, hori } // vert: �����ړ�, hori: �����ړ�
    Status MovementMode; // ���݂̍s�����[�h

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        HoriMoveSpeed = NormalMoveSpeed;

        // �o���ʒu�ɂ���ď����̐����ړ�����������
        if (transform.position.x > -6.5 && transform.position.x < 6.5)
        {
            HoriMoveSpeed = 0; // �����t�߂Ȃ琅���ړ��Ȃ�
        }
        else if (transform.position.x > 6.5)
        {
            HoriMoveSpeed = -HoriMoveSpeed; // �E�[�Ȃ獶��
        }
        // ���[�Ȃ炻�̂܂܂̑��x(�E��)
       
        MovementMode = enemy.Status.vert; // �������[�h�͐����ړ�
        bulletTimer = 0;
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // Y���W��-1�ȉ��ɂȂ����琅���ړ����[�h�Ɉڍs
        if(transform.position.y <= -1)
        {
            MovementMode = Status.hori;
        }

        // �����ړ����[�h�̏ꍇ
        if (MovementMode == Status.vert)
        {
            // �ݒ肳�ꂽ���x�Ŏ΂߉��܂��͐^���Ɉړ�
            transform.Translate(HoriMoveSpeed * Time.deltaTime, -NormalMoveSpeed * Time.deltaTime, 0.0f);
        }
        
        // �����ړ����[�h�̏ꍇ
        if (MovementMode == Status.hori)
        {
            // ��ʒ������獶�E�ɕ�����ĉ�ʊO�ֈړ�
            if (transform.position.x >= 0)
            {
                transform.Translate(NormalMoveSpeed * Time.deltaTime, 0.0f, 0.0f); // �E��
            }
            if (transform.position.x < 0)
            {
                transform.Translate(-NormalMoveSpeed * Time.deltaTime, 0.0f, 0.0f); // ����
            }
        }

        // �e���˃^�C�}�[���X�V
        bulletTimer += Time.deltaTime;

        // ���ˊԊu�ɒB������
        if(bulletTimer >= bulletInterval)
        {
            // �e�𐶐�
            Instantiate(bulletPrefab, transform.position, Quaternion.identity);
            // �^�C�}�[���Z�b�g
            bulletTimer = 0.0f;
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