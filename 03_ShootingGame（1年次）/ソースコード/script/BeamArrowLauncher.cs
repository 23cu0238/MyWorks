// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// BeamArrowLauncher.cs
// �T�u����u�r�[���A���[�v�̔��˂��Ǘ�����N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BeamArrowLauncher : MonoBehaviour
{
    [SerializeField] GameObject BA; // �r�[���A���[�̃v���n�u
    CD cD; // �N�[���_�E���^�C�}�[�̏����Ǘ�����R���|�[�l���g
    public float cooldown; // �N�[���_�E���̎���
    public float cdTimer; // ���݂̃N�[���_�E���^�C�}�[

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        cdTimer = 0; // �N�[���_�E���^�C�}�[��������
        
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
            
            // �T�u���픭�˃L�[�i�E�N���b�N�j�������ꂽ���`�F�b�N
            if (Input.GetKeyDown(KeyCode.Mouse1))
            {
                // �V�[�����̃r�[���A���[�iPlayerSubBullet�^�O�j������
                GameObject[] ba = GameObject.FindGameObjectsWithTag("PlayerSubBullet");

                // �ݒu�ς݂̃r�[���A���[��5�����̏ꍇ
                if (ba.Length < 5)
                {
                    // �V�����r�[���A���[�𐶐�
                    Instantiate(BA, transform.position, Quaternion.identity);
                }
                // �ݒu�ς݂̃r�[���A���[��5�ȏ�̏ꍇ
                else if (ba.Length >= 5)
                {
                    // �S�Ă̐ݒu�ς݃r�[���A���[�𔭎ˁi�A�N�e�B�u��ԂɁj����
                    foreach (GameObject sc in ba)
                    {
                        if (sc != null) // �O�̂���null�`�F�b�N
                        {
                            sc.GetComponent<beamArrow>().mode = beamArrow.Mode.active;
                        }
                    }
                    // �N�[���_�E���^�C�}�[���J�n
                    cdTimer = cooldown;
                }
            }
        }

        // CD�R���|�[�l���g�Ɍ��݂̃N�[���_�E���^�C�}�[��n��
        if (cD != null)
        {
            cD.cd = cdTimer;
        }
    }
}