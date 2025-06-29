// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// DamageRED.cs
// �_���[�W���󂯂��ۂɃX�v���C�g��Ԃ��_�ł�����N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DamageRED : MonoBehaviour
{
    SpriteRenderer sr; // �X�v���C�g�`��p�R���|�[�l���g
    public bool Change; // �F��ύX�����ǂ����̃t���O
    public float REDTime; // �ԐF�\���̌o�ߎ���
    
    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        Change = false;
        REDTime = 0;
        sr = GetComponent<SpriteRenderer>();
    }

    // ���t���[���Ă΂�܂�
    private void Update()
    {
        // �F���ύX���̏ꍇ
        if (Change == true)
        {
            REDTime += Time.deltaTime; // �o�ߎ��Ԃ����Z

            // 1�b�ȉ��̊ԁA�F��Ԃ݂��������F�ɕύX
            if (REDTime > 0 && REDTime <= 0.1f) // �������Ԃ�Z������
            {
                sr.color = new Color(1f, 0.46f, 0.52f, 1f); // RGB�l��0-1�͈̔͂Ŏw��
            }
            // 0.1�b�𒴂����猳�̐F�ɖ߂�
            else if (REDTime > 0.1f)
            {
                sr.color = Color.white; // new Color(1, 1, 1, 1) �Ɠ���
                Change = false; // �t���O�����Z�b�g
                REDTime = 0; // �^�C�}�[�����Z�b�g
            }
        }
    }

    // ���̃R���C�_�[�ƐڐG�����u�ԂɌĂ΂�܂�
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // �v���C���[�̒e�ɓ��������ꍇ
        if (collision.gameObject.tag == "PlayerBullet") 
        {
            // �F�ύX�t���O�𗧂Ă�
            Change = true;
            REDTime = 0; // �^�C�}�[���Z�b�g
        }
    }
}