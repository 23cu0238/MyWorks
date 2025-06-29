// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// BossHoming.cs
// �{�X�����˂���ǔ��e�̋����𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BossHoming : MonoBehaviour
{
    public float moveSpeed; // �e�̈ړ����x
    public GameObject player; // �v���C���[�I�u�W�F�N�g�ւ̎Q��
    Rigidbody2D rigidBody; // Rigidbody2D�R���|�[�l���g
    public float angleChangingSpeed; // �ǔ����̐��񑬓x
    // float distance; // ���g�p�̂��߃R�����g�A�E�g

    // �e�̍s�����[�h���`����񋓑�
    public enum Mode
    {
        chasing, // �ǔ����[�h
        stright  // ���i���[�h
    }

    public Mode mode; // ���݂̃��[�h

    // �v���C���[�����������ꍇ�Ɏg�p����ꎞ�I�ȖڕW�ʒu
    Vector2 tempPos;
    Vector2 NullPos = new Vector2(float.PositiveInfinity, float.PositiveInfinity); // �����Ȉʒu�������萔

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player");
        rigidBody = GetComponent<Rigidbody2D>();
        mode = Mode.chasing; // �������[�h�͒ǔ�
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // Z����]���Œ肷��i2D�Q�[���ł̈Ӑ}���Ȃ���]��h���j
        transform.rotation = new Quaternion(0f, 0f, transform.rotation.z, transform.rotation.w);

        // �v���C���[�𖈃t���[������
        player = GameObject.FindGameObjectWithTag("Player");
        
        // Y���W��1�ȉ��ɂȂ�����A�ǔ�����߂Ē��i���[�h�Ɉڍs
        if(transform.position.y <= 1)
        { 
            mode = Mode.stright;
        }

        // �ǔ����[�h�̏ꍇ
        if (mode == Mode.chasing)
        {
            // �v���C���[�����݂���ꍇ
            if (player != null)
            {
                tempPos = NullPos; // �ꎞ�ڕW�ʒu�����Z�b�g
                // �v���C���[�ւ̕����x�N�g�����v�Z
                Vector2 direction = (Vector2)player.transform.position - rigidBody.position;
                direction.Normalize();
                // ���݂̑O���x�N�g���ƖڕW�����Ƃ̊p�x�����v�Z
                float rotateAmount = Vector3.Cross(direction, transform.up).z;
                // �p�x���ɉ����Ċp���x��ݒ肵�A���񂳂���
                rigidBody.angularVelocity = -angleChangingSpeed * rotateAmount;
                // ���݂̑O�������ɑ��x��ݒ�
                rigidBody.velocity = transform.up * moveSpeed;
            }
            // �v���C���[�����݂��Ȃ��ꍇ�i���j���ꂽ�Ȃǁj
            else if (player == null)
            {
                // �v���C���[�����������u�Ԃ̈ʒu���ꎞ�ڕW�Ƃ��ĕۑ�
                if (tempPos == NullPos)
                {
                    tempPos = transform.position + transform.up * 10; // �b��I�ɑO����
                }
                // �ꎞ�ڕW�Ɍ������Ĉړ�
                Vector2 direction = tempPos - rigidBody.position;
                direction.Normalize();
                float rotateAmount = Vector3.Cross(direction, transform.up).z;
                rigidBody.angularVelocity = -angleChangingSpeed * rotateAmount;
                rigidBody.velocity = transform.up * moveSpeed;
            }
        }
        // ���i���[�h�̏ꍇ
        else
        {
            // ������~���A���݂̌����̂܂ܒ��i
            rigidBody.angularVelocity = 0;
            rigidBody.velocity = transform.up * moveSpeed;
        }
    }

    // ��ʊO�ɏo���玩�g��j��
    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "ActiveArea")
        {
            Destroy(gameObject);
        }
    }
}