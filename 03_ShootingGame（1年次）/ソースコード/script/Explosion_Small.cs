// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Explosion_Small.cs
// �����������A�j���[�V�������I�������玩�g��j������N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Explosion_Small : MonoBehaviour
{
    Animator anim; // �A�j���[�^�[�R���|�[�l���g

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        anim = GetComponent<Animator>();
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // ���ݍĐ����̃A�j���[�V�������I�������� (normalizedTime��1�ȏ�ɂȂ�����)
        if (anim.GetCurrentAnimatorStateInfo(0).normalizedTime >= 1.0f)
        {
            // ���̃Q�[���I�u�W�F�N�g��j������
            Destroy(gameObject);
        }
    }
}