// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���� 23cu0238���N�F
// ProcessManager.cs
// �X�e�[�W�̐i�s�x���Ǘ�����N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProcessManager : MonoBehaviour
{
    public int target; // �ڕW�ƂȂ�i�s�x
    public int process; // ���݂̐i�s�x

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // process�̏��������K�v�Ȃ炱���ōs��
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        
    }

    // �i�s�x�����Z����֐�
    public void ProcessPlus(int value)
    {
        process += value;
    }
}