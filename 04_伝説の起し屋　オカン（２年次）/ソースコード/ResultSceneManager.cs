//---------------------------------------------------------------------------
//�쐬�ҁF���N�F
//�쐬�����F2024/10/11
//�T�v�F���U���g�V�[���𐧌䂷��}�l�[�W���[
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;



public class ResultSceneManager : MonoBehaviour
{
    [SerializeField] Animator mTextAnimator;        //�e�L�X�g�̃A�j���[�^�[
    [SerializeField] Text mText;                    //�e�L�X�g�̃R���|�[�l���g
    [SerializeField] GameObject mClearIllustration; //�N���A�̃J�b�g�C���摜�I�u�W�F�N�g

    // Start is called before the first frame update
    void Start()
    {
        //�A�j���V�����Đ����A�e�L�X�g�ϊ�
        mTextAnimator.Play("Start",0);
        mText.text = ("�I");
    }

    // Update is called once per frame
    void Update()
    {
        //�A�j���V���������I�Ȏ��Ԉȏ�Đ����ꂽ��
        if (mTextAnimator.GetCurrentAnimatorStateInfo(0).normalizedTime >= 0.6)
        {
            //�N���A�̏ꍇ
            if(GameManager.instance.gameStatus == GameManager.GameStatus.clear)
            {
                mText.gameObject.SetActive(false);
                mClearIllustration.SetActive(true);
            }
            //�Q�[���I�[�o�[�̏ꍇ
            else
            {
                mText.text = "���s";
            }
        }

        //�Đ�����
        if (mTextAnimator.GetCurrentAnimatorStateInfo(0).normalizedTime >= 1)
        {
            //�Q�[���}�l�[�W���[�ɒʒm
            GameManager.instance.bIsResultTextAnimFinished = true;
        }
    }
}
