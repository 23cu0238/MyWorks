// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Boss.cs
// �G�L�X�g���X�e�[�W�̃{�X�̍s���p�^�[���𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Boss : MonoBehaviour
{
    public float EndTimer; // ���j��̏I���V�[�P���X�p�^�C�}�[
    public float moveSpeed; // �ړ����x
    public float boundaries; // ���E�̈ړ����E
    public GameObject bulletPrefab; // ���˂���e�̃v���n�u
    public GameObject Ending; // �G���f�B���O���o�p�̃v���n�u
    public float bulletInterval; // �e�̔��ˊԊu
    Vector3 bulletTransform; // �e�̔��ˈʒu
    Animator animator; // �A�j���[�^�[�R���|�[�l���g

    // �{�X�̈ʒu���
    public enum Bpos
    {
        center,
        left,
        right
    };
    // �{�X�̈ړ�����
    public enum Bdirection
    {
        towardsLeft,
        towardsRight
    };
    // �{�X�̍s�����
    public enum Bstatus
    {
        moving,
        stop
    }

    public Bpos BossPos; // ���݂̈ʒu���
    public Bdirection BossDirection; // ���݂̈ړ�����
    public Bstatus BossStatus; // ���݂̍s�����
    bool isShot; // �U���A�j���[�V�������Ɉ�񂾂��e�𔭎˂��邽�߂̃t���O
    public bool IsEnd; // ���j���ꂽ���ǂ����̃t���O
    SpriteRenderer spr; // �X�v���C�g�`��p�R���|�[�l���g
    Vector3 centerPos; // �����ʒu�̕ۑ��p

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        animator = GetComponent<Animator>();
        // �e��Ԃ̏�����
        BossPos = Bpos.center;
        BossDirection = Bdirection.towardsLeft;
        BossStatus = Bstatus.moving;
        isShot = false;
        IsEnd = false;
        spr = GetComponent<SpriteRenderer>();
        // �����ʒu��ۑ�
        centerPos = new Vector3(0.0f,transform.position.y,transform.position.z);
        EndTimer = 0;
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // ���j����Ă��Ȃ��ꍇ
        if (IsEnd == false)
        {
            // === �����ɂ���Ƃ��̏��� ===
            if (BossPos == Bpos.center)
            {
                // �ړ����̏ꍇ
                if (BossStatus == Bstatus.moving)
                {
                    // ���ֈړ�
                    if (BossDirection == Bdirection.towardsLeft)
                    {
                        if (transform.position.x > -boundaries)
                        {
                            transform.Translate(-moveSpeed, 0.0f, 0.0f);
                            isShot = false;
                        }
                        // ���[�ɓ��B�������~���A�U���A�j���[�V�����Đ�
                        if (transform.position.x <= -boundaries)
                        {
                            animator.Play("orga_shoot");
                            BossPos = Bpos.left;
                            BossStatus = Bstatus.stop;
                        }
                    }
                    // �E�ֈړ�
                    if (BossDirection == Bdirection.towardsRight)
                    {
                        if (transform.position.x < boundaries)
                        {
                            transform.Translate(moveSpeed, 0.0f, 0.0f);
                            isShot = false;
                        }
                        // �E�[�ɓ��B�������~���A�U���A�j���[�V�����Đ�
                        if (transform.position.x >= boundaries)
                        {
                            animator.Play("orga_shoot");
                            BossPos = Bpos.right;
                            BossStatus = Bstatus.stop;
                        }
                    }
                }
                // ��~���ŁA���U���ς݂̏ꍇ
                else if (BossStatus == Bstatus.stop && isShot == true)
                {
                    // �A�C�h���A�j���[�V�����ɖ߂�����A�ēx�ړ����J�n
                    if (spr.sprite.name == "ezgif.com-gif-maker_1") // ����̃X�v���C�g���Ɉˑ�����͕̂s����ȉ\������
                    {
                        BossStatus = Bstatus.moving;
                    }
                }
            }

            // === ���[�ɂ���Ƃ��̏��� ===
            if (BossPos == Bpos.left)
            {
                // ��~���ŁA���U���ς݂̏ꍇ
                if (BossStatus == Bstatus.stop)
                {
                     // �A�C�h���A�j���[�V�����ɖ߂�����A�E�ֈړ����J�n
                    if (spr.sprite.name == "ezgif.com-gif-maker_0" && isShot == true)
                    {
                        BossDirection = Bdirection.towardsRight;
                        BossStatus = Bstatus.moving;
                    }
                }
                // �ړ����̏ꍇ
                if (BossStatus == Bstatus.moving)
                {
                    // �����Ɍ������Ĉړ�
                    if (transform.position.x < boundaries)
                    {
                        transform.Translate(moveSpeed, 0.0f, 0.0f);
                        isShot = false;
                    }
                    // �����ɓ��B�������~���A�U���A�j���[�V�����Đ�
                    if (transform.position.x >= 0.0f)
                    {
                        animator.Play("orga_shoot");
                        transform.position = centerPos;
                        BossPos = Bpos.center;
                        BossStatus = Bstatus.stop;
                    }
                }
            }
            
            // === �E�[�ɂ���Ƃ��̏��� ===
            if (BossPos == Bpos.right)
            {
                // ��~���ŁA���U���ς݂̏ꍇ
                if (BossStatus == Bstatus.stop)
                {
                    // �A�C�h���A�j���[�V�����ɖ߂�����A���ֈړ����J�n
                    if (spr.sprite.name == "ezgif.com-gif-maker_0" && isShot == true)
                    {
                        BossDirection = Bdirection.towardsLeft;
                        BossStatus = Bstatus.moving;
                    }
                }
                 // �ړ����̏ꍇ
                if (BossStatus == Bstatus.moving)
                {
                    // �����Ɍ������Ĉړ�
                    if (transform.position.x > -boundaries)
                    {
                        transform.Translate(-moveSpeed, 0.0f, 0.0f);
                        isShot = false;
                    }
                     // �����ɓ��B�������~���A�U���A�j���[�V�����Đ�
                    if (transform.position.x <= 0.0f)
                    {
                        animator.Play("orga_shoot");
                        transform.position = centerPos;
                        BossPos = Bpos.center;
                        BossStatus = Bstatus.stop;
                    }
                }
            }

            // �U���A�j���[�V�����̓���t���[���Œe�𔭎�
            if (spr.sprite.name == "orga_shoot_25" && isShot == false)
            {
                Shoot();
            }
        }
        // ���j���ꂽ��̏���
        else if (IsEnd == true)
        {
            EndTimer += Time.deltaTime;
            // 5�b��ɃG���f�B���O���o�𐶐�
            if (EndTimer >= 5)
            {
                Instantiate(Ending, new Vector3(0, 0, 0), Quaternion.identity);
                EndTimer = 0; // �����񐶐����Ȃ��悤�Ƀ��Z�b�g
            }    
        }
    }

    // �e�𔭎˂���֐�
    public void Shoot()
    {
        bulletTransform = new Vector3(transform.position.x, transform.position.y, transform.position.z);
        // 3-way�e�𔭎�
        Instantiate(bulletPrefab, bulletTransform, Quaternion.identity);
        Instantiate(bulletPrefab, bulletTransform, Quaternion.AngleAxis(30, new Vector3(0.0f, 0.0f, 1.0f)));
        Instantiate(bulletPrefab, bulletTransform, Quaternion.AngleAxis(-30, new Vector3(0.0f, 0.0f, 1.0f)));

        isShot = true; // ���ˍς݃t���O�𗧂Ă�
    }
    
    // ���j�V�[�P���X���J�n����֐�
    public void EndEx()
    {
        IsEnd = true; // ���j�t���O�𗧂Ă�
        animator.Play("orga_down"); // �_�E���A�j���[�V�������Đ�
    }
}