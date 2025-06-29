// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// PowerUpIcon.cs
// UI��ɃT�u�E�F�|���̃A�C�R����\������N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PowerUpIcon : MonoBehaviour
{
    SpriteRenderer sr; // �A�C�R���\���p��SpriteRenderer
    public Sprite[] sprite; // �T�u�E�F�|�����Ƃ̃X�v���C�g�z��
    
    Player player; // �v���C���[�R���|�[�l���g�ւ̎Q��

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player")?.GetComponent<Player>();
        sr = GetComponent<SpriteRenderer>();
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �v���C���[�I�u�W�F�N�g�����݂��邩�m�F
        if (GameObject.FindGameObjectWithTag("Player") != null)
        {
            // �v���C���[�R���|�[�l���g���Ď擾�i�v���C���[�����X�|�[�������ꍇ�̂��߁j
            player = GameObject.FindGameObjectWithTag("Player").GetComponent<Player>();
            if (player != null && sprite.Length > player.SubWeaponIndex && player.SubWeaponIndex >= 0)
            {
                // �v���C���[�̌��݂̃T�u����C���f�b�N�X�ɉ����ăX�v���C�g��؂�ւ���
                sr.sprite = sprite[player.SubWeaponIndex];
            }
        }
    }
}