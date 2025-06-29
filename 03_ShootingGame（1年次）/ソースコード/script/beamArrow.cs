// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// beamArrow.cs
// �v���C���[�̃T�u����u�r�[���A���[�v�̋����𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class beamArrow : MonoBehaviour
{
    public float movespeed; // �r�[���A���[�̈ړ����x
    public int power; // �r�[���A���[�̍U����
    SpriteRenderer sr; // �X�v���C�g�`��p�̃R���|�[�l���g

    // �r�[���A���[�̏�Ԃ��`����񋓑�
    public enum Mode
    {
        idle,   // �ҋ@���
        active, // ���ˌ�̃A�N�e�B�u���
    };

    public Mode mode; // ���݂̏��

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // ������Ԃ�ҋ@�ɐݒ�
        mode = Mode.idle;
        // SpriteRenderer�R���|�[�l���g���擾
        sr = GetComponent<SpriteRenderer>();
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �A�N�e�B�u��Ԃ̏ꍇ
        if (mode == Mode.active)
        {
            // �F��Ԃɐݒ肵�A�O���ɒ��i������
            sr.color = new Color(1, 0, 0, 1);
            transform.Translate(0, movespeed, 0);
        }
        // �ҋ@��Ԃ̏ꍇ
        else if (mode == Mode.idle)
        {
            // PingPong�֐����g���ĐF�𖾖ł�����
            sr.color = new Color(Mathf.PingPong(Time.time, 1), 0, 0, 1);
        }
    }

    // ���̃R���C�_�[�ƐڐG�����u�ԂɌĂ΂�܂�
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // �A�N�e�B�u��Ԃł̂ݓ����蔻����s��
        if (mode == Mode.active)
        {
            // �G�܂��̓{�X�Ƀq�b�g�����ꍇ
            if (collision.gameObject.tag == "Enemy" || collision.gameObject.tag == "Boss")
            { 
                // �����HP�R���|�[�l���g���擾���A�_���[�W��^����
                HP hit = collision.gameObject.GetComponent<HP>();
                if (hit != null) // ���S�̂���null�`�F�b�N
                {
                    hit.Damage(power);
                }
            }
        }
    }

    // ���̃R���C�_�[���A�N�e�B�u�G���A����o���u�ԂɌĂ΂�܂�
    private void OnTriggerExit2D(Collider2D collision)
    {
        // ��ʊO�ɏo���玩�g��j������
        if (collision.gameObject.tag == "ActiveArea")
        {
            Destroy(gameObject);
        }
    }
}