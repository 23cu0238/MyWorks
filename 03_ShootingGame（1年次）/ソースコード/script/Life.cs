// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Life.cs
// �v���C���[���擾���郉�C�t�A�b�v�A�C�e���̋����𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Life : MonoBehaviour
{
    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {

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