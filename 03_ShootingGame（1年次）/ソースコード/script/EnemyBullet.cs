// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// EnemyBullet.cs
// �G�����˂���e�̋����𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyBullet : MonoBehaviour
{
    public float moveSpeed; // �e�̈ړ����x

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // Z����]�Ɋ�Â��āA���g�̑O���i���[�J�����W��Y���������j�ֈړ�
        transform.Translate(0.0f, -moveSpeed * Time.deltaTime, 0.0f);
    }

    // ��ʊO�iActiveArea�̃R���C�_�[����o���j�ɏo���玩�g��j��
    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "ActiveArea")
        {
            Destroy(gameObject);
        }
    }
}