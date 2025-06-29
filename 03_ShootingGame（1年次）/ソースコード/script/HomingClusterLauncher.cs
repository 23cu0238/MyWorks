// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// HomingClusterLauncher.cs
// �T�u����u�z�[�~���O�N���X�^�[�v�̔��˂��Ǘ�����N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HomingClusterLauncher : MonoBehaviour
{
    [SerializeField] GameObject HC; // �z�[�~���O�N���X�^�[�̃v���n�u
    CD cD; // �N�[���_�E�������Ǘ�����R���|�[�l���g
    public int ClusterNum; // ��x�ɔ��˂���~�T�C���̐�
    public float cooldown; // �N�[���_�E������
    public float cdTimer; // ���݂̃N�[���_�E���^�C�}�[

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        cdTimer = 0; // �^�C�}�[������
        
        // CD�R���|�[�l���g���擾���A�N�[���_�E�����Ԃ�ݒ�
        cD = GetComponent<CD>();
        if (cD != null)
        {
            cD.CooldownAmount = cooldown;
        }
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �N�[���_�E���^�C�}�[�����Z
        if (cdTimer > 0)
        {
            cdTimer -= Time.deltaTime;
        }
        else if (cdTimer <= 0)
        {
            cdTimer = 0; // �}�C�i�X�ɂȂ�Ȃ��悤�ɒ���
            
            // �T�u���픭�˃L�[�i�E�N���b�N�j��������Ă����
            if (Input.GetKey(KeyCode.Mouse1))
            {
                // �w�肳�ꂽ���̃~�T�C������x�ɐ���
                for (int i = 0; i < ClusterNum; ++i)
                {
                    Instantiate(HC, transform.position, Quaternion.identity);
                }
                // �N�[���_�E���^�C�}�[���J�n
                cdTimer = cooldown;
            }
        }

        // ���݂̃N�[���_�E���^�C�}�[��CD�R���|�[�l���g�ɓn��
        if (cD != null)
        {
            cD.cd = cdTimer;
        }
    }
}