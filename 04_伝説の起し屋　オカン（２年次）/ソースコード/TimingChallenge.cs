//---------------------------------------------------------------------------
//�쐬�ҁF���N�F
//�쐬�����F2024/10/11
//�T�v�F�^�C�~���O�̃~�j�Q�[���h���N���X
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TimingChallenge : ChallengesBase
{
    Scrollbar mScrollbar;                   //����Q�[�W
    [SerializeField] float mGreatZoneRange; //great�̔���͈�
    [SerializeField] float mGoodZoneRange;  //good�̔���͈�

    // Start is called before the first frame update
    protected override void Start()
    {
        //��{�N���X�̃X�^�[�g����
        base.Start();
        //���
        mScrollbar = transform.GetChild(2).GetComponent<Scrollbar>();
    }

    // Update is called once per frame
    protected override void Update()
    {
        //���ʂ��o�Ă��Ȃ����̏���
        if (mChallengeResult == ChallengeResult.none)
        {
            //�J�[�\�����ړ�������
            mScrollbar.value = Mathf.PingPong(Time.time * mChallengeAttributes, 1.0f);
            //�g���{�^���������ꂽ��
            if (Input.GetButtonDown("Interaction"))
            {
                //���ʔ���
                if (mScrollbar.value > 0.5f - mGreatZoneRange / 2.0f && mScrollbar.value < 0.5f + mGreatZoneRange / 2.0f)
                {
                    mChallengeResult = ChallengeResult.great;
                }
                else if (mScrollbar.value > 0.5f - mGoodZoneRange / 2.0f && mScrollbar.value < 0.5f + mGoodZoneRange / 2.0f)
                {
                    mChallengeResult = ChallengeResult.good;
                }
                else
                {
                    mChallengeResult = ChallengeResult.bad;
                }
            }
        }
        //��{�N���X�̍X�V����
        base.Update();
    }
}
