// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// LaserBeam.cs
// �v���C���[�̃��[�U�[����̋����𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LaserBeam : MonoBehaviour
{
    public int power; // ���[�U�[�̕b�ԃ_���[�W
    private GameObject player; // �v���C���[�I�u�W�F�N�g�ւ̎Q��
    float hitInterval = 0.1f; // �_���[�W��^����Ԋu
    float hitCountdown; // ���̃_���[�W�܂ł̃J�E���g�_�E���^�C�}�[

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player");
        hitCountdown = 0;
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �v���C���[�����݂���ꍇ
        if(player != null)
        {
            // �v���C���[�̈ʒu�ɍ��킹�Ď��g�̈ʒu���X�V����
            Vector3 newPos = new Vector3(player.transform.position.x, player.transform.position.y + 7.0f, 0.0f);
            transform.position = newPos;
        }
        else
        {
            // �v���C���[�����Ȃ��ꍇ�͎��g���j��
            Destroy(gameObject);
            return;
        }

        // �_���[�W�Ԋu�^�C�}�[���X�V
        hitCountdown += Time.deltaTime;
    }

    // ���̃R���C�_�[�ƐڐG���Ă���ԁA���t���[���Ă΂�܂�
    private void OnTriggerStay2D(Collider2D collision)
    {
        // �_���[�W�Ԋu���o�߂��Ă���ꍇ
        if (hitCountdown >= hitInterval)
        {
            // �ڐG���肪�G�܂��̓{�X�̏ꍇ
            if (collision.gameObject.tag == "Enemy" || collision.gameObject.tag == "Boss")
            {
                // �����HP�R���|�[�l���g���擾���A�_���[�W��^����
                HP hit = collision.gameObject.GetComponent<HP>();
                if (hit != null)
                {
                    hit.Damage(power);
                }
                // �_���[�W��^������^�C�}�[�����Z�b�g
                hitCountdown = 0;
            }
        }
    }
}