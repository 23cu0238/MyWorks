// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// shield.cs
// �v���C���[����莞�Ԏ��V�[���h�̋����𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class shield : MonoBehaviour
{
    // �V�[���h�̎������ԃ^�C�}�[
    float Timer;
    
    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // �^�C�}�[��2�b�ɏ�����
        Timer = 2;
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // ���Ԍo�߂Ń^�C�}�[������
        Timer -= Time.deltaTime;
        // �^�C�}�[��0�ȉ��ɂȂ�����V�[���h��j��
        if (Timer<=0)
        {
            Destroy(gameObject);
        }
    }

    // ���̃R���C�_�[�ƐڐG�������ɌĂ΂�܂�
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // �ڐG�������肪�G�̏ꍇ
        if (collision.gameObject.tag == "Enemy")
        {
            // �G�ɑ�_���[�W��^���Ĕj�󂷂�
            HP hit = collision.gameObject.GetComponent<HP>();
            hit.Damage(999);
        }
        // �ڐG�������肪�G�̒e�̏ꍇ
        if (collision.gameObject.tag == "EnemyBullet")
        {
            // �G�̒e��j�󂷂�
            Destroy(collision.gameObject);
        }
    }
}