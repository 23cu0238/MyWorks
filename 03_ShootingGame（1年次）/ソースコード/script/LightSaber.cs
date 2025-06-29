// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// LightSaber.cs
// �T�u����u���C�g�Z�[�o�[�v�̋����𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor; // UnityEditor���O��Ԃ̓r���h���ɃG���[�ɂȂ邽�߁A�ʏ�͎g�p���Ȃ����A#if UNITY_EDITOR�ň͂�

public class LightSaber : MonoBehaviour
{
    public float power; // ���C�g�Z�[�o�[�̍U����
    public float rotation; // ��]���x

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {

    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // ���C�����픭�˃L�[�i���N���b�N�j��������Ă����
        if (Input.GetKey(KeyCode.Mouse0))
        {
            // Z������ɉ�]������
            transform.Rotate(new Vector3(0, 0, rotation * Time.deltaTime));
        }
    }

    // ���̃R���C�_�[�ƐڐG�����u�ԂɌĂ΂�܂�
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // �G�܂��̓{�X�Ƀq�b�g�����ꍇ
        if (collision.gameObject.tag == "Enemy" || collision.gameObject.tag == "Boss")
        {
            // ����Ƀ_���[�W��^����
            HP hit = collision.gameObject.GetComponent<HP>();
            if (hit != null)
            {
                hit.Damage(power);
            }
        }
    }
}