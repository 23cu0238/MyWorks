// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// cooldownTimer.cs
// �T�u����̃N�[���_�E����Ԃ�UI��Image�Ŏ��o�I�ɕ\������N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class cooldownTimer : MonoBehaviour
{
    Image image; // �N�[���_�E���\���p��UI Image�R���|�[�l���g
    public CD sub; // �T�u����̃N�[���_�E������ێ�����CD�R���|�[�l���g

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        image = GetComponent<Image>();
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �V�[�����ɃT�u���탉���`���[�����݂��邩�m�F
        GameObject subWeaponLauncher = GameObject.FindGameObjectWithTag("SubWeaponLauncher");
        if (subWeaponLauncher != null)
        {
            // �T�u���탉���`���[����CD�R���|�[�l���g���擾
            sub = subWeaponLauncher.GetComponent<CD>();
            if (sub != null && sub.CooldownAmount > 0)
            {
                // �c��N�[���_�E�����Ԃ̊������v�Z���AImage��fillAmount�ɐݒ�
                image.fillAmount = sub.cd / sub.CooldownAmount;
            }
            else
            {
                image.fillAmount = 0;
            }
        }
        else 
        {
            // �T�u���탉���`���[���Ȃ��ꍇ��fillAmount��0�ɂ���
            image.fillAmount = 0;
        }
    }
}