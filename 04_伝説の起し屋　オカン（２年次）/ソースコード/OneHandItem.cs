//---------------------------------------------------------------------------
//�쐬�ҁF���N�F
//�쐬�����F2024/10/04
//�T�v�F�Ў莝���A�C�e���h���N���X
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OneHandItem : ItemBase
{
    bool bIsCombo = false; //�����Ă���A�C�e���̓x�X�g�}�b�`���ǂ���

    public OneHandItem mOtherHandItem;  //�ʂ̎�Ɏ����Ă���A�C�e��

    // Start is called before the first frame update
    protected override void Start()
    {
        //������
        mOtherHandItem = null;
        //��{�N���X�̃X�^�[�g����
        base.Start();
    }

    // Update is called once per frame
    protected override void Update()
    {
        //��{�N���X�̍X�V����
        base.Update();
    }

    /// <summary>
    /// �Ў莝���A�C�e�����g���Ƃ��Ăяo��
    /// </summary>
    /// <param name="_isCombo">�����Ă���A�C�e���̓x�X�g�}�b�`���ǂ���</param>
    public override void Interact(bool _isCombo)
    {
        //��{�N���X�̊֐����Ăяo��
        base.Interact(bIsCombo);    

        //�N�����̃~�j�Q�[�������݂��Ȃ���ΐ�������
        if(mActiveChallenge == null)
        {
            mActiveChallenge = Instantiate(mChallengePrefab, mUICanvas.transform);
        }

        //��Փx�␳�W�����Z�b�g����
        mActiveChallenge.GetComponent<ChallengesBase>().mChallengeAttributes = mChallengeAttributes;
        //�g�p�σt���O���Z�b�g����
        mActiveChallenge.GetComponent<ChallengesBase>().mUsedObj = gameObject;

        //�x�X�g�}�b�`���ǂ����������o�[�ϐ��ɑ��
        bIsCombo = _isCombo;
    }

    /// <summary>
    /// �~�j�Q�[���̌��ʂ��R�[���o�b�N
    /// </summary>
    /// <param name="result">���ʂ�enum</param>
    public override void CallResult(ChallengesBase.ChallengeResult result)
    {
        //�����ʂ̎���A�C�e���������Ă���΂������Ăяo��
        if (mOtherHandItem != null)
        {
            mOtherHandItem.CallResult(result);
        }

        //�A�N�e�B�u�ȃ~�j�Q�[���̎Q�Ƃ�����
        if (mActiveChallenge != null)
        {
            mActiveChallenge = null;
        }

        //���U���g�ɉ����ď���
        switch(result)
        {
            case ChallengesBase.ChallengeResult.great:
                mAwakePowMultiplier += 0.5f;    //�o���␳�W��+50��
                break;
            case ChallengesBase.ChallengeResult.good:
                break;
            case ChallengesBase.ChallengeResult.bad:
                mAnnoyingPowMultiplier += 0.2f; //���f�␳�W��+20��
                break;
        }

        //�x�X�g�}�b�`�ł����
        if (bIsCombo)
        {
            mAwakePowMultiplier += 0.5f;�@//�o���␳�W��+50��
        }
        
        //�Q�[���}�l�[�W���[���̏���
        GameManager.instance.InteractWithTarget(mAwakePow * mAwakePowMultiplier, mAnnoyingPow * mAnnoyingPowMultiplier);
        
        //�e�␳�W�������Z�b�g
        mAwakePowMultiplier = mAnnoyingPowMultiplier = 1.0f;
        //�ʂ̎�̎Q�Ƃ�����
        mOtherHandItem = null;
        //��{�N���X�̊֐����Ăяo��
        base.CallResult(result);
    }
}
