// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// EMPExplosion.cs
// EMP�A�C�e���擾���ɔ������锚���̋����𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EMPExplosion : MonoBehaviour
{
    private float timer; // �o�ߎ��ԃ^�C�}�[
    public float ExpandSpeed; // �������g�傷�鑬�x
    public float DestroyTime; // ������������܂ł̎���
    public float power; // �������G�ɗ^����_���[�W

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        timer = 0;
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        timer += Time.deltaTime; // �o�ߎ��Ԃ����Z
        
        // ���g�̃X�P�[�����g�傳���āA�������L����悤�Ɍ�����
        transform.localScale += new Vector3(ExpandSpeed, ExpandSpeed, ExpandSpeed) * Time.deltaTime; // ���x���t���[�����[�g�Ɉˑ����Ȃ��悤�ɏC��

        // �w�莞�Ԃ��o�߂����玩�g��j��
        if(timer >= DestroyTime)
        {
            Destroy(gameObject);
        }
    }

    // ���̃R���C�_�[�ƐڐG�����u�ԂɌĂ΂�܂�
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // ��ʓ��̓G�A�܂��̓{�X�Ƀq�b�g�����ꍇ
        if ((collision.gameObject.tag == "Enemy" && collision.gameObject.transform.position.y <= 4.5) || collision.gameObject.tag == "Boss")
        {
            // �����HP�R���|�[�l���g���擾���A�_���[�W��^����
            HP hit = collision.gameObject.GetComponent<HP>();
            if (hit != null)
            {
                hit.Damage(power);
            }
        }
        // �G�̒e�Ƀq�b�g�����ꍇ
        if(collision.gameObject.tag == "EnemyBullet")
        {
            // �e������
            Destroy(collision.gameObject);
        }
    }
}