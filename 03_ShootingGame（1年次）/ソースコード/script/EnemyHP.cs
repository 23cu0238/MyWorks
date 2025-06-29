// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// EnemyHP.cs
// �G��HP��UI�X���C�_�[�iHP�o�[�j�ɔ��f������N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class EnemyHP : MonoBehaviour
{
    Slider HB; // HP�o�[�Ƃ��Ďg�p����Slider�R���|�[�l���g

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        HB = GetComponent<Slider>(); // �������g��Slider�R���|�[�l���g���擾
        
        // �e�I�u�W�F�N�g������HP�R���|�[�l���g����ő�HP�ƌ���HP���擾���A�X���C�_�[�ɐݒ�
        HP parentHP = GetComponentInParent<HP>();
        if(parentHP != null)
        {
            HB.maxValue = parentHP.enemyHp;
            HB.value = parentHP.enemyHp;
        }
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �e�I�u�W�F�N�g��HP����ɃX���C�_�[�̒l�ɔ��f������
        HP parentHP = GetComponentInParent<HP>();
        if(parentHP != null)
        {
            HB.value = parentHP.enemyHp;
        }
        // �e���j�󂳂ꂽ�ꍇ�A����HP�o�[����\���܂��͔j�����鏈����ǉ�����Ɨǂ�
        else
        {
            // ��: gameObject.SetActive(false);
        }
    }
}