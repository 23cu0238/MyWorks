//---------------------------------------------------------------------------
//�쐬�ҁF���N�F
//�쐬�����F2024/10/16
//�T�v�F�A���[�Y�̃~�j�Q�[���h���N���X
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using UnityEditor;
using Unity.VisualScripting;

public class DirectionChallenge : ChallengesBase
{
    [SerializeField] GameObject mArrowPrefab;   //�A���[�̃v���n�u
    List<GameObject> mArrowList;                //�������ꂽ�A���[���X�g
    GameObject mIconCanvas;                     //�A���[�̒u����

    bool bIsDPadReleased;   //�R���g���[���̏\���L�[��������Ă��Ȃ����ǂ���

    // Start is called before the first frame update
    protected override void Start()
    {
        //��{�N���X�̃X�^�[�g���Ăяo��
        base.Start();

        //�ϐ�������
        bIsDPadReleased = true;
        mArrowList = new List<GameObject>();

        //�������
        mIconCanvas = transform.GetChild(2).gameObject;

        //�A���[����
        //���̃N���X��mChallengeAttributes=���̐�
        for (int i = 0; i < (int)mChallengeAttributes; i++)
        {
            //�������A���X�g�ɒǉ�
            GameObject newArrow = Instantiate(mArrowPrefab, mIconCanvas.transform);
            mArrowList.Add(newArrow);

            //�����_���ɐ���
            //��̃v���n�u�̂ݎg�p
            //��]�Ɩ��O�ō��ʉ�������
            switch((int)Random.Range(0,4))
            {
                case 0:
                    newArrow.name = "Up";
                    newArrow.transform.localRotation = Quaternion.Euler(0, 0, -90) ;
                    break;

                case 1:
                    newArrow.name = "Down";
                    newArrow.transform.localRotation = Quaternion.Euler(0, 0, 90);
                    break;

                case 2:
                    newArrow.name = "Right";
                    newArrow.transform.localRotation = Quaternion.Euler(0, 0, 180);
                    break;

                case 3:
                    newArrow.name = "Left";
                    break;

            }

            //�A���[��u����ɕ��u���A����������
            newArrow.transform.localPosition = new Vector3(
                (mChallengeAttributes / 2.0f) * -150.0f + 75.0f + i * 150.0f, 0);
        }
            
    }

    // Update is called once per frame
    protected override void Update()
    {
        //���ʂ��o�Ă��Ȃ����̏���
        if (mChallengeResult == ChallengeResult.none)
        {
            if((bIsDPadReleased &&  //�\���L�[���������ꂽ��
                (
                //WASD�L�[/�\���L�[��������āA��ԑO�̃A���[�ƍ����Ă��鎞�̏���
                ((Input.GetButtonDown("ArrowUp") || Input.GetAxis("DPadY") > 0 )&& mArrowList[0].transform.name == "Up")
                || ((Input.GetButtonDown("ArrowLeft") || Input.GetAxis("DPadX") < 0) && mArrowList[0].transform.name == "Left")
                || ((Input.GetButtonDown("ArrowDown") || Input.GetAxis("DPadY") < 0) && mArrowList[0].transform.name == "Down")
                || ((Input.GetButtonDown("ArrowRight")|| Input.GetAxis("DPadX") > 0) && mArrowList[0].transform.name == "Right")
                )))
            {
                //�\���L�[�������ꂽ���Ƃ��L�^
                bIsDPadReleased = false;
                //�A���[�̏���
                ChangeArrow();
            }
            else if (    //WASD�L�[/�\���L�[��������āA��ԑO�̃A���[�ƍ����Ă��Ȃ����̏���
                Input.GetButtonDown("ArrowUp")
                || Input.GetButtonDown("ArrowLeft")
                || Input.GetButtonDown("ArrowDown")
                || Input.GetButtonDown("ArrowRight")
                || Input.GetAxis("DPadX") != 0
                || Input.GetAxis("DPadY") != 0
                )
            {
                //�A���[��Ԃ�����
                mArrowList[0].GetComponent<SpriteRenderer>().color = Color.red;
                
                //���ʂ𔻒�
                if (mArrowList.Count < (int)mChallengeAttributes / 2)   //�����ȏ�̃A���[������������good
                {
                    mChallengeResult = ChallengeResult.good;
                }
                else�@  //�����ȉ���bad
                {
                    mChallengeResult = ChallengeResult.bad;
                }
            }

            //�\���L�[�������ꂽ���ǂ����̔���
            if(Input.GetAxis("DPadX") == 0 && Input.GetAxis("DPadY") == 0)
            {
                bIsDPadReleased = true;
            }

            //�S�����m�ɉ����ꂽ��great
            if(mArrowList.Count <= 0)
            {
                mChallengeResult = ChallengeResult.great;
            }
        }

        //��{�N���X�̍X�V
        base.Update();
    }

    /// <summary>
    /// �A���[�̐F��΂ɂ��A���̃A���[�ɖڕW�ύX
    /// </summary>
    private void ChangeArrow()
    {
        mArrowList[0].GetComponent<SpriteRenderer>().color = Color.green;
        mArrowList.RemoveAt(0);
    }

    /// <summary>
    /// �f�o�b�O�p
    /// </summary>
    void OnGUI()
    {
        if (Application.isEditor)  // or check the app debug flag
        {
            GUI.Label(new Rect(10, 10, 1000, 2000), bIsDPadReleased.ToString() 
                + "Y:"+Input.GetAxis("DPadY").ToString()
                + " X:" + Input.GetAxis("DPadX").ToString() + "\nJoystick Name:" + Input.GetJoystickNames()[0]);
        }
    }
}
