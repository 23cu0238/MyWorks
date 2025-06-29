// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// BGScroll.cs
// �w�i���c�ɃX�N���[��������N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BGScroll : MonoBehaviour
{
    public float scrollSpeed; // �X�N���[�����x
    private Vector3 startPosition; // �����ʒu

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // �I�u�W�F�N�g�̏����ʒu��ۑ�
        startPosition = transform.position;
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {        
        // Mathf.Repeat ���g���āA�l��0����10�̊Ԃ����[�v����悤�ɐV����Y���W���v�Z
        // ����ɂ��A�w�i�����͈̔͂��J��Ԃ��X�N���[������悤�ɂȂ�܂�
        float newPosition = Mathf.Repeat(Time.time * scrollSpeed, 20.48f); // ���[�v�̒����𒲐� (��: 20.48)
        
        // �����ʒu����v�Z���ꂽ�V����Y���W���������Ɉړ�������
        transform.position = startPosition + Vector3.down * newPosition;
    }
}