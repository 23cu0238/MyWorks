// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// PlayerDoubleBullet.cs
// �v���C���[�̃_�u���o���b�g�i���g�p���J�����j�̋����𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerDoubleBullet : MonoBehaviour
{
    public float moveSpeed; // �e�̈ړ����x
    public GameObject explosionPrefab; // �����G�t�F�N�g�̃v���n�u
    // public int bulletPower; // �U���́i�R�����g�A�E�g����Ă���j

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
        // �G�Ƀq�b�g�����ꍇ
        if (collision.gameObject.tag == "Enemy")
        {
            // ���g��j��
            Destroy(gameObject);
            // �����œG�Ƀ_���[�W��^���鏈�����K�v
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