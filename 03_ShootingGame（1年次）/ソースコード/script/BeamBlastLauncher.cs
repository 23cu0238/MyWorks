// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// BeamBlastLauncher.cs
// ���C������u�r�[���u���X�g�v�̔��˂��Ǘ�����N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BeamBlastLauncher : MonoBehaviour
{
    [SerializeField] GameObject[] BB; // �v���C���[�̃��[�h�ɉ������r�[���u���X�g�̃v���n�u�z��
    Player player; // �v���C���[�R���|�[�l���g�ւ̎Q��
    public float cooldown; // �N�[���_�E������
    public float cdTimer; // ���݂̃N�[���_�E���^�C�}�[

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // �e�I�u�W�F�N�g����Player�R���|�[�l���g���擾
        player = GetComponentInParent<Player>();
        cdTimer = 0; // �N�[���_�E���^�C�}�[��������
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �N�[���_�E���^�C�}�[�����Z
        cdTimer -= Time.deltaTime;

        // �N�[���_�E�����I�����Ă���ꍇ
        if (cdTimer <= 0)
        {
            // ���C�����픭�˃L�[�i���N���b�N�j��������Ă����
            if (Input.GetKey(KeyCode.Mouse0))
            {
                // �v���C���[�̌��݂̃��[�h�ɉ������r�[���u���X�g�𐶐�
                if (player != null && BB.Length > player.mode)
                {
                     Instantiate(BB[player.mode], transform.position, Quaternion.identity);
                     // �N�[���_�E���^�C�}�[�����Z�b�g
                     cdTimer = cooldown;
                }
            }
        }
    }
}