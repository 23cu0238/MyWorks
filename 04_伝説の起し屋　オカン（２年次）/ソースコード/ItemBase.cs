//---------------------------------------------------------------------------
//�쐬�ҁF���N�F
//�쐬�����F2024/10/04
//�T�v�F�A�C�e���̊�{�N���X
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class ItemBase : MonoBehaviour
{
    [SerializeField] Material mDefaultMat;  //�f�t�H���g�̃}�e���A��
    [SerializeField] Material mOutlineMat;  //�g���Ɉ͂܂��}�e���A��

    protected GameObject mUICanvas;         //UI�J���o�X
    [SerializeField] protected GameObject mChallengePrefab; //�~�j�Q�[���̃v���n�u
    protected GameObject mActiveChallenge;  //�A�N�e�B�u�ȃ~�j�Q�[��
    
    Transform mPlayer;  //�v���C���[�̃g�����X�t�H�[��
    
    [SerializeField] protected Vector3 mStartPos;   //�����ʒu
    
    [SerializeField] protected float mAwakePow;     //�o���p���[
    public float AwakePow { get {return mAwakePow; } }  //�ǂݎ���p
    
    protected float mOriginalAwakePow;  //�p���[�̏����l
    public float OriginalAwakePow { get { return mOriginalAwakePow; } } //�ǂݎ���p

    [SerializeField] protected float mAnnoyingPow;  //���f�p���[
    
    [SerializeField] float mHeight; //�n�ʂ���̍���
    public float Height { get => mHeight; }//�ǂݎ���p

    protected float mAwakePowMultiplier;    //�o���p���[�̕␳�W��
    protected float mAnnoyingPowMultiplier; //���f�p���[�̕␳�W��
    float mResetTimer;  //���Z�b�g����O�̎c�莞��
    [SerializeField] protected float mChallengeAttributes;  //�~�j�Q�[���̓�Փx�W��

    [SerializeField] readonly string mComboName; //�x�X�g�}�b�`�̃A�C�e����
    public string ComboName { get => mComboName; }�@//�ǂݎ���p
    
    bool IsUsed = false;    //�g��ꂽ���ǂ���

    // Start is called before the first frame update
    protected virtual void Start()
    {
        //�e�ϐ��̏������Ƒ��
        mUICanvas = GameManager.instance.UICanvas;
        mStartPos = transform.position;
        mOriginalAwakePow = mAwakePow;
        mAwakePowMultiplier = mAnnoyingPowMultiplier = 1.0f;
    }

    // Update is called once per frame
    protected virtual void Update()
    {
        //�g���̍X�V����
        UpdateOutline();

        //�v���C���[�ɏE��ꂽ��
        if (transform.parent != null)
        {
            mResetTimer = 0;
            GetComponentInChildren<SpriteRenderer>().color = new Color(1, 1, 1, 1);
        }
        
        //�v���C���[�Ɏ̂Ă�ꂽ��
        else if (transform.parent == null && IsUsed) Reset();
    }

    /// <summary>
    /// �g���̍X�V����
    /// </summary>
    void UpdateOutline()
    {
        //�G���[�h�~
        if(mPlayer == null) return;
        
        //�������g�̓v���C���[�̍ł��߂��A�C�e���ł���Θg����\��
        if (mPlayer.GetComponent<PlayerController>().mClosestItem == gameObject)
        {
            gameObject.GetComponentInChildren<SpriteRenderer>().material = mOutlineMat;
        }
        else
        {
            gameObject.GetComponentInChildren<SpriteRenderer>().material = mDefaultMat;
        }

        //�E��ꂽ��g������������
        if (GetComponent<BoxCollider>().enabled == false)
        {
            gameObject.GetComponentInChildren<SpriteRenderer>().material = mDefaultMat;
        }
    }


    private void OnTriggerStay(Collider other)
    {
        //�v���C���[�ƐڐG���鎞����̎Q�Ƃ�o�^
        if (other.gameObject.CompareTag("Player"))
        {
            mPlayer = other.gameObject.transform;
        }
    }

    /// <summary>
    /// ���Z�b�g����
    /// </summary>
    private void Reset()
    {
        //���Z�b�g����܂ł̎��Ԃ��v��
        mResetTimer += Time.deltaTime;
        
        //5�b�o��
        if(mResetTimer > 5.0f)
        {
            //�摜���_��
            GetComponentInChildren<SpriteRenderer>().color = new Color(1, 1, 1, Mathf.PingPong(Time.time, 1.0f));
        }

        //10�b�o�߂����烊�Z�b�g
        if(mResetTimer > 10.0f)
        {
            mPlayer = null;
            transform.position = mStartPos;
            mAwakePow = mOriginalAwakePow;
            IsUsed = false;
            mResetTimer = 0;
            GetComponentInChildren<SpriteRenderer>().color = new Color(1, 1, 1, 1);
        }
    }

    /// <summary>
    /// �A�C�e�����g���Ƃ��Ăяo��
    /// </summary>
    /// <param name="_isCombo">�����Ă���A�C�e���̓x�X�g�}�b�`���ǂ���</param>
    public virtual void Interact(bool _isCombo)
    {
         IsUsed = true;
    }

    /// <summary>
    /// �~�j�Q�[���̌��ʂ��R�[���o�b�N
    /// </summary>
    /// <param name="result">���ʂ�enum</param>
    public virtual void CallResult(ChallengesBase.ChallengeResult result)
    {
        //�p���[�𔼌�
        mAwakePow = mAwakePow * 0.5f;
    }
}
