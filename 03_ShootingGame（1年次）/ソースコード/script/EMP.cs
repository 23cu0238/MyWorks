// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// EMP.cs
// �v���C���[���擾����EMP�A�C�e���̋����𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EMP : MonoBehaviour
{
    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // ����������
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �X�V����
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