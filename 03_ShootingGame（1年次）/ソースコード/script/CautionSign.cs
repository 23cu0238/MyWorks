// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// CautionSign.cs
// �{�X�o���O�̌x���T�C���̕\���Ɠ_�ł𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CautionSign : MonoBehaviour
{
    SpriteRenderer sr; // �X�v���C�g�`��p�R���|�[�l���g
    float timer; // �\�����Ԃ��Ǘ�����^�C�}�[

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        sr = GetComponent<SpriteRenderer>();
        timer = 0;
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        timer += Time.deltaTime;

        // PingPong�֐����g���ăA���t�@�l�i�����x�j��0��1�̊Ԃŉ��������A�_�ł�����
        sr.color = new Color(1, 1, 1, Mathf.PingPong(Time.time * 2, 1)); // ���x�𒲐�

        // 3�b�o�߂����玩�g��j��
        if (timer >= 3)
        {
            Destroy(gameObject);
        }
    }
}