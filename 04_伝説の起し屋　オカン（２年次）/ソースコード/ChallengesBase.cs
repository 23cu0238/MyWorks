//---------------------------------------------------------------------------
//�쐬�ҁF���N�F
//�쐬�����F2024/10/11
//�T�v�F�~�j�Q�[���I�u�W�F�N�g�̊�{�N���X
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ChallengesBase : MonoBehaviour
{
    public enum ChallengeResult //�~�j�Q�[���̌��ʂ�ENUM��`
    {
        none,
        great,
        good,
        bad
    }

    protected ChallengeResult mChallengeResult;�@//�~�j�Q�[���̌���

    [SerializeField] GameObject mResultTextObj;�@//�~�j�Q�[���̌��ʂ�\�����镶���I�u�W�F�N�g
    public GameObject mUsedObj;                  //���̃~�j�Q�[���𐶐������A�C�e���̃I�u�W�F�N�g
    [SerializeField] float mScrollSpeed;         //�����I�u�W�F�N�g�̈ړ����x
    Text mResultText;                            //�~�j�Q�[���̌��ʂ�\�����镶��

    public float mChallengeAttributes;           //�~�j�Q�[���̎�ނɂ���ĐF��Ȗ��������l�ɕϊ��p
    private float mIdleTimer;                    ////�����I�u�W�F�N�g�̈ړ���~�������Ԃ𑪂�

    // Start is called before the first frame update
    protected virtual void Start()
    {
        //������
        mResultText = mResultTextObj.GetComponent<Text>();
        mIdleTimer = 0;
    }

    // Update is called once per frame
    protected virtual void Update()
    {
        //���ʂ��o�����̏���
        if (mChallengeResult != ChallengeResult.none)
        {
            //�����I�u�W�F�N�g���N��
            mResultTextObj.SetActive(true);
            
            //���ʂɂ���ĕ����̕��򏈗�
            //great:�p���[�@+50%, ��
            //good:�{�[�i�X�Ȃ�,�@���F
            //bad:���f+20%,       ��
            switch (mChallengeResult)
            { 
                case ChallengeResult.great:
                    mResultText.text = "Great!\nPower +50%";
                    mResultText.color = Color.green;
                    break;

                case ChallengeResult.good:
                    mResultText.text = "Good!";
                    mResultText.color = Color.yellow;
                    break;

                case ChallengeResult.bad:
                    mResultText.text = "Bad!\nAnnoying +20%";
                    mResultText.color = Color.red;
                    break;

                default:
                    break;
            }
            
            //�����I�u�W�F�N�g�̈ړ�����
            float newYpos = Mathf.Clamp(mResultTextObj.transform.localPosition.y - mScrollSpeed * Time.deltaTime, 115.0f,1000.0f);
            mResultTextObj.transform.localPosition = new Vector3(mResultTextObj.transform.localPosition.x,
                           newYpos,
                           -10.0f);

            //�w��̈ʒu�ɓ��B������
            if(newYpos <= 115.0f)
            {
                //���Ԍv��
                mIdleTimer += Time.deltaTime;
                
                //��b�o�߂�����
                if (mIdleTimer > 1.0f)
                {
                    //���ʂ��A�C�e���I�u�W�F�N�g�ƃQ�[���}�l�[�W���[�ɒʒm
                    mUsedObj.GetComponent<ItemBase>().CallResult(mChallengeResult);
                    GameManager.instance.NotifyChallenge(false);
                    Destroy(gameObject);    //����
                }
            }
        }

    }
}
