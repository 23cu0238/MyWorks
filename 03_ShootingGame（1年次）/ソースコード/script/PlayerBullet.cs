// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// PlayerBullet.cs
// �v���C���[�����˂���ʏ�e�̋����𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerBullet : MonoBehaviour
{
    public float moveSpeed; // �e�̈ړ����x
    public float power; // �e�̍U����

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // ���g��Y���������i������j�ֈړ�
        transform.Translate(0.0f, moveSpeed * Time.deltaTime, 0.0f);
    }

    // ���̃R���C�_�[�ƐڐG�����u�ԂɌĂ΂�܂�
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // �G�܂��̓{�X�Ƀq�b�g�����ꍇ
        if (collision.gameObject.tag == "Enemy" || collision.gameObject.tag == "Boss")
        {
            // ���g��j��
            Destroy(gameObject);

            // �����HP�R���|�[�l���g���擾���A�_���[�W��^����
            HP hit = collision.gameObject.GetComponent<HP>();
            if (hit != null)
            {
                hit.Damage(power);
            }
        }
    }

    // ��ʊO�iActiveArea�j�ɏo���玩�g��j������
    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "ActiveArea")
        {
            Destroy(gameObject);
        }
    }
}