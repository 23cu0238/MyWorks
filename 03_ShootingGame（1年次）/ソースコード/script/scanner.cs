// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���� 23cu0238���N�F
// scanner.cs
// ����̃I�u�W�F�N�g�i�e�j�ɒǏ]����@�\�����N���X�i����͕s���S�j

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class scanner : MonoBehaviour
{
    GameObject parents; // �e�I�u�W�F�N�g
    MoveTowardsPlayer MTP; // (���g�p)

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // Start���Őe�I�u�W�F�N�g��ݒ肷�郍�W�b�N���K�v
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
       // �e�I�u�W�F�N�g���ݒ肳��Ă���΁A���̈ʒu�ɒǏ]����
        if (parents != null)
        {
            transform.position = parents.transform.position;
        }
    }
}