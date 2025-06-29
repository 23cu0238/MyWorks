// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// LaserLauncher.cs
// ���[�U�[����̔��˃G�t�F�N�g�i�p�[�e�B�N���j�𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LaserLauncher : MonoBehaviour
{
    public GameObject laser; // ���[�U�[�{�̂̃v���n�u
    ParticleSystem ps; // �p�[�e�B�N���V�X�e��
    ParticleSystem.ColorOverLifetimeModule pscm; // �p�[�e�B�N���̐F�i���Ԍo�߁j���W���[��
    [SerializeField] Gradient[] _gradient; // �v���C���[�̃��[�h�ɉ������F�̃O���f�[�V�����z��
    Player player; // �v���C���[�R���|�[�l���g�ւ̎Q��

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        ps = GetComponent<ParticleSystem>();
        pscm = ps.colorOverLifetime; // �F���W���[�����擾
        
        // ���[�U�[�{�̂𐶐�
        if (laser != null)
        {
            Instantiate(laser, transform.position, Quaternion.identity);
        }

        // �e�I�u�W�F�N�g����Player�R���|�[�l���g���擾
        player = GetComponentInParent<Player>();
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        if (player == null) return; // �v���C���[�����Ȃ��ꍇ�͏������f

        // �v���C���[�̌��݂̃��[�h�ɉ����ăp�[�e�B�N���̐F��ݒ�
        if (_gradient.Length > player.mode)
        {
            pscm.color = new ParticleSystem.MinMaxGradient(_gradient[player.mode]);
        }

        // �p�[�e�B�N���̕��˃��W���[�����擾
        var emission = ps.emission;

        // ���C�����픭�˃L�[�i���N���b�N�j��������Ă����
        if (Input.GetKey(KeyCode.Mouse0))
        {
            // �p�[�e�B�N���̕��˂�L���ɂ���
            emission.enabled = true;
        }
        else
        {
            // �p�[�e�B�N���̕��˂𖳌��ɂ���
            emission.enabled = false;
        }
    }
}