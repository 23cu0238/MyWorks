// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// ProcessBar.cs
// �X�e�[�W�̐i�s�x��UI��Slider�ŕ\������N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ProcessBar : MonoBehaviour
{
    ProcessManager pm; // �i�s�x���Ǘ�����ProcessManager
    Slider slider; // �i�s�x�\���p��Slider�R���|�[�l���g
    
    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // Main Camera�I�u�W�F�N�g����ProcessManager�R���|�[�l���g���擾
        pm = GameObject.Find("Main Camera").GetComponent<ProcessManager>();
        // �������g��Slider�R���|�[�l���g���擾
        slider = GetComponent<Slider>();

        if(pm != null)
        {
            // �X���C�_�[�̍ő�l��ڕW�i�s�x�ɐݒ�
            slider.maxValue = pm.target;
            // �X���C�_�[�̏����l��0�ɐݒ�
            slider.value = 0;
        }
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // ProcessManager�̌��݂̐i�s�x���X���C�_�[�̒l�ɔ��f
        if (pm != null && slider != null)
        {
            slider.value = pm.process;
        }
    }
}