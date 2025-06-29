// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// MoveTowardsPlayer.cs
// �v���C���[�Ɍ������Ĉړ�����G�̋����𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveTowardsPlayer : MonoBehaviour
{
    public float moveSpeed; // �ړ����x
    GameObject player; // �v���C���[�I�u�W�F�N�g�ւ̎Q��

    float distance; // �v���C���[�Ƃ̋���

    // �G�̍s�����[�h
    public enum Mode
    {
        initial, // �����ʒu�ւ̈ړ��t�F�[�Y
        moving,  // �v���C���[�ւ̒ǐՃt�F�[�Y
    };

    public Mode mode; // ���݂̃��[�h
    float timer; // ���[�h�ڍs�p�̃^�C�}�[

    public Vector3 startPos; // �����ړ��̖ڕW�ʒu

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player");
        mode = Mode.initial;
        
        // ��ʏ㕔�A���g��X���W�t�߂Ƀ����_���ȏ����ڕW�ʒu��ݒ�
        Vector2 randPos = Random.insideUnitCircle * 1;
        if (randPos.x < -6.2f) randPos.x = -6.2f;
        if (randPos.x > 6.2f) randPos.x = 6.2f;
        startPos = new Vector3(randPos.x + transform.position.x, 4.5f, 0); // Y���W����ʏ㕔�ɏC��
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        timer += Time.deltaTime;
        // 1�b��ɒǐՃ��[�h�Ɉڍs
        if (timer >= 1)
        {
            mode = Mode.moving;
        }

        // �v���C���[�����Ȃ��ꍇ�͉������Ȃ�
        if (GameObject.FindGameObjectWithTag("Player") == null)
        {
            // �v���C���[�����Ȃ��ꍇ�A���̏�Œ�~����Ȃǂ̋�����ǉ����Ă��悢
            return;
        }
        else
        {
            player = GameObject.FindGameObjectWithTag("Player");
        }
        
        // �v���C���[�Ƃ̋������v�Z
        distance = Vector3.Distance(transform.position, player.transform.position);

        // �ǐՃ��[�h�̏ꍇ
        if(mode == Mode.moving)
        { 
            // �v���C���[�Ƃ̋������߂��ꍇ�A���x���グ�Ēǐ�
            if (distance <= 2.0f)
            {
                transform.position = Vector2.Lerp(transform.position, player.transform.position, moveSpeed * 2 * Time.deltaTime);
            }
            // �v���C���[�Ƃ̋����������ꍇ�A�^���Ɉړ�
            if (distance > 2.0f)
            {
                transform.Translate(0.0f, -moveSpeed * Time.deltaTime, 0.0f);
            } 
        }
        // �����ړ��t�F�[�Y�̏ꍇ
        else
        {
            // �ݒ肳�ꂽ�����ڕW�ʒu�Ɍ������Ĉړ�
            Vector2 dir = (Vector2)startPos - (Vector2)transform.position;
            transform.Translate(dir.normalized * 2 * Time.deltaTime, Space.World);
        }
    }
}