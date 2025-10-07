//---------------------------------------------------------------------------
//�쐬�ҁF���N�F
//�쐬�����F2024/10/11
//�T�v�F�d�b�A�C�e���̃N���X
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Phone : MonoBehaviour
{
    bool bIsAvalible;       //�g���邩�ǂ���
    public bool bIsUsed;    //��x�g��ꂽ���ǂ���
    bool bIsActive;         //�N�����ꂽ���ǂ���

    GameManager mGameManager;   //�Q�[���}�l�[�W���[

    Material mMat;  //�}�e���A��

    Animator mAnimator; //�A�j���[�^�[
    
    // Start is called before the first frame update
    void Start()
    {
        //������
        bIsAvalible = false;
        bIsUsed = false;
        bIsActive = false;

        //�������
        mGameManager = GameManager.instance;
        mAnimator = GetComponent<Animator>();
        mMat = GetComponent<SpriteRenderer>().material; 
    }

    // Update is called once per frame
    void Update()
    {
        //�N�����ꂽ���ǂ����̃`�F�b�N
        ActivateCheck();
        //�A�j���V�����̍X�V����
        AnimatorUpdate();

        //�N�����ꂽ���g����Ƃ�
        if (bIsAvalible && bIsActive)
        {
            //�g�p�`�F�b�N�i���͑҂��j
            InteractionCheck();
            //�g����\��
            mMat.SetFloat("_OutValue", 0.2f);
        }
        else
        {
            //�g�����\��
            mMat.SetFloat("_OutValue", 0f);
        }
    }

    /// <summary>
    /// �N�������`�F�b�N
    /// </summary>
    void ActivateCheck()
    {
        //�N�������F���f�x��60������ꂽ���Ƃ��Ȃ�
        if (mGameManager.AnnoyingValue > 60 && !bIsUsed)
        {
            bIsActive = true;
        }
        else
        {
            bIsActive = false;
        }
    }

    /// <summary>
    /// �A�j���V�����̍X�V����
    /// </summary>
    void AnimatorUpdate()
    {
        ///�N�����ł���΃A�j���V�����Đ�
        if (bIsActive)
        {
            mAnimator.Play("Ringing");
        }
        else
        {
            mAnimator.Play("Idle");
        }
    }

    /// <summary>
    /// �g���{�^���������ꂽ���ǂ������`�F�b�N
    /// </summary>
    void InteractionCheck()
    {
        if(Input.GetButtonDown("Interaction"))
        {
            //�g���ƁF���f�x-30�A�g��ꂽ�t���O���g�D���[�AUI�ɕ\��
            mGameManager.InteractWithTarget(0, -30);
            mGameManager.bIsPlayingMinusAnnoyTextAnim = true;
            bIsUsed = true;
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        //�v���C���[�ƐڐG����Ǝg����悤��
        if(other.gameObject.CompareTag("Player"))
        {
            bIsAvalible = true;
        }
    }

    private void OnTriggerExit(Collider other)
    {
        //�v���C���[���痣���Ǝg���Ȃ��悤��
        if (other.gameObject.CompareTag("Player"))
        {
            bIsAvalible = false;
        }
    }
}
