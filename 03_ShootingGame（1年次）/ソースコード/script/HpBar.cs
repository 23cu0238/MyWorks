// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// HpBar.cs
// �{�X��HP��UI��Slider��Text�ŕ\������N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class HpBar : MonoBehaviour
{
    Slider HB; // HP�o�[�{�̂�Slider�R���|�[�l���g
    public Text BossHPtext; // HP���e�L�X�g�ŕ\������UI Text�R���|�[�l���g

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        HB = GetComponent<Slider>();
        
        // �V�[����̃{�X�I�u�W�F�N�g��T��
        GameObject boss = GameObject.FindGameObjectWithTag("Boss");
        if(boss != null)
        {
            // �{�X��HP�R���|�[�l���g����ő�HP���擾���A�X���C�_�[�̍ő�l�ƌ��ݒl�ɐݒ�
            HP bossHpComponent = boss.GetComponent<HP>();
            if(bossHpComponent != null)
            {
                HB.maxValue = bossHpComponent.enemyHp;
                HB.value = bossHpComponent.enemyHp;
                // �e�L�X�g���X�V
                BossHPtext.text = "Boss HP:" + (int)HB.value;
            }
        }
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �{�X�I�u�W�F�N�g��T��
        GameObject boss = GameObject.FindGameObjectWithTag("Boss");
        if(boss != null)
        {
            // �{�X�̌��݂�HP���X���C�_�[�ƃe�L�X�g�ɔ��f
            HP bossHpComponent = boss.GetComponent<HP>();
            if(bossHpComponent != null)
            {
                HB.value = bossHpComponent.enemyHp;
                BossHPtext.text = "Boss HP: " + (int)HB.value;
            }
        }
        else
        {
            // �{�X�����Ȃ��i�|���ꂽ�j�ꍇ�AHP�o�[��UI���\���ɂ���
            gameObject.SetActive(false);
        }
    }
}