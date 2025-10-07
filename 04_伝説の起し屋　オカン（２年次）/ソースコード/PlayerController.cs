//---------------------------------------------------------------------------
//�쐬�ҁF���N�F
//�쐬�����F2024/10/04
//�T�v�F�v���C���[�𐧌䂷��R���g���[��
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.ProBuilder.MeshOperations;
using UnityEngine.UI;

public class PlayerController : MonoBehaviour
{
    enum Hands�@//��̒�`
    {
        left,   //��
        right,  //�E
    }

    Hands mCurrentHand = Hands.left;    //�I�𒆂̎�
    
    [SerializeField] float mMoveSpeed;  //�ړ����x
   
    GameObject mLeftItem = null;    //����A�C�e��
    GameObject mRightItem = null;   //�E��A�C�e��
    public GameObject mClosestItem; //�ł��߂��A�C�e��
    [SerializeField] GameObject mLeftItemBox;   //����A�C�e����UI
    [SerializeField] GameObject mRightItemBox;  //�E��A�C�e����UI
    [SerializeField] GameObject mBestMatchText; //�x�X�g�}�b�`�̕���
    [SerializeField] Slider mLeftItemPowerBar;  //����A�C�e���̎c��p���[�Q�[�W
    [SerializeField] Slider mRightItemPowerBar; //�E��A�C�e���̎c��p���[�Q�[�W
    [SerializeField] Sprite mFrontSprite;       //���ʉ摜
    [SerializeField] Sprite mBackSprite;        //�w�ʉ摜
    SpriteRenderer mCurrentSprite;              //���݂̉摜

    GameManager mGameManager;�@//�Q�[���}�l�[�W���[

    bool bIsNearTarget = false;     //�a���ɂ��邩�ǂ���
    bool bIsItemACombo = false;     //�����Ă���A�C�e�����x�X�g�}�b�`���ǂ���
    bool bIsNearInteractableObject; //�C���^�[�A�N�g�\�ȃA�C�e���̋߂��ɂ��邩�ǂ���

    // Start is called before the first frame update
    void Start()
    {
        //�������
        mCurrentSprite = transform.GetChild(0).GetComponent<SpriteRenderer>();
        mGameManager = GameManager.instance;
    }

    // Update is called once per frame
    void Update()
    {
        //�v���C��
        if(mGameManager.gameStatus == GameManager.GameStatus.playing)
        {
            //�摜�X�V����
            SpriteUpdate();
            //�ړ�����
            Movement();
            //���I�����鏈��
            HandSelect();
            //�A�C�e���̍X�V����
            ItemUpdate();
        }

        //�v���C���ł͂Ȃ��Ƃ��͒�~
        else 
        {
            GetComponent<Rigidbody>().velocity = Vector3.zero;
        }
    }  

    /// <summary>
    /// �摜�̍X�V����
    /// </summary>
    void SpriteUpdate()
    {
        if(Input.GetAxis("Vertical") > 0 && (mCurrentSprite.sprite != mBackSprite))
        {
            mCurrentSprite.sprite = mBackSprite;
        }

        else if (Input.GetAxis("Vertical") < 0 && (mCurrentSprite.sprite != mFrontSprite))
        {
            mCurrentSprite.sprite = mFrontSprite;
        }

        if (Input.GetAxis("Horizontal") > 0 && (mCurrentSprite.flipX == false))
        {
            mCurrentSprite.flipX = true;
        }

        else if (Input.GetAxis("Horizontal") < 0 && (mCurrentSprite.flipX == true))
        {
            mCurrentSprite.flipX = false;
        }
    }

    /// <summary>
    /// �ړ�����
    /// </summary>
    void Movement()
    {      
        GetComponent<Rigidbody>().velocity = new Vector3(
            Input.GetAxis("Horizontal") * mMoveSpeed * Time.deltaTime
            , 0
            ,Input.GetAxis("Vertical") * mMoveSpeed * Time.deltaTime);   
    }

    /// <summary>
    /// �A�C�e���̍X�V����
    /// </summary>
    void ItemUpdate()
    {
        //����Ƃ��A�C�e���������Ă���
        if(mLeftItem != null && mRightItem != null)
        {
            //�x�X�g�}�b�`�`�F�b�N
            if(mLeftItem.GetComponent<ItemBase>().ComboName == mRightItem.name)
            {
                bIsItemACombo = true;
                mBestMatchText.SetActive(true);
            }
        }

        else�@//��΃x�X�g�}�b�`�ɂȂ�Ȃ��̂�
        {
            
            bIsItemACombo = false;
            mBestMatchText.SetActive(false);
        }

        //�A�C�e���E������
        if (Input.GetButtonDown("Pickup"))
        {
            //�I�����Ă���肪�󂢂Ă��違�ł��A�C�e��������
            if (IsSelectedHandIsFree()&& mClosestItem != null)
            {
                //�E��
                ChangeCurrentHandItem(mClosestItem);
                ItemPickUp();
            }
            //�I�����Ă���肪�󂢂Ă��Ȃ����ł��A�C�e�������݁��ʂ̎肪�󂢂Ă���
            else if (!IsSelectedHandIsFree() && mClosestItem != null && IsOtherHandIsFree())
            {
                //�E��
                ChangeOtherHandItem(mClosestItem);
                ItemPickUp();
            }
        }

        //�A�C�e�����g��
        if(Input.GetButtonDown("Interaction"))
        {
            //�I�𒆂̎肪�A�C�e���������Ă��違�a���ɂ��違�߂��ɃC���^�[�A�N�g�\�ȃA�C�e�����Ȃ�
            if (!IsSelectedHandIsFree() && bIsNearTarget && !bIsNearInteractableObject)
            {
                //��ɂ���͕̂Ў莝���A�C�e���ł����
                if(GetCurrentHandItem().TryGetComponent<OneHandItem>(out OneHandItem Currentitem))
                {
                    //�ʂ̎肪�A�C�e���������Ă��違���̎�ɂ���͕̂Ў莝���A�C�e���ł����
                    if (!IsOtherHandIsFree() && GetOtherHandItem().TryGetComponent<OneHandItem>(out OneHandItem Otheritem))
                    {
                        //�Q�Ƃ�o�^
                        Currentitem.mOtherHandItem = Otheritem;
                    }
                    //�����ł͂Ȃ����
                    else
                    {
                        //�Q�Ƃ�����
                        Currentitem.mOtherHandItem = null;
                    }
                }
                
                //�g���֐����Ăяo��
                GetCurrentHandItem().GetComponent<ItemBase>().Interact(bIsItemACombo);
                //�Q�[���}�l�[�W���[��ʒm
                GameManager.instance.NotifyChallenge(true);
            }
        }

        //�̂Ă鏈��
        if (Input.GetButtonDown("Cancel"))
        {
            GameObject CancelItem = null;
            //�̂Ă����A�C�e�����w��
            if (!IsSelectedHandIsFree())
            {
                CancelItem = GetCurrentHandItem().gameObject;               
            }
            else if (!IsOtherHandIsFree())
            {
                SwitchHand();
                CancelItem = GetCurrentHandItem().gameObject;
                
            }

            //�̂Ă����A�C�e�������݂����
            if (CancelItem != null)
            {
                //�R���C�_�[�Ɖ摜�`���L����
                CancelItem.GetComponent<BoxCollider>().enabled = true;
                CancelItem.GetComponentInChildren<SpriteRenderer>().enabled = true;
                //�e�q�֌W������
                CancelItem.transform.parent = null;
                //�ʒu�𒲐�
                CancelItem.transform.position =
                    new Vector3(CancelItem.transform.position.x,
                        CancelItem.transform.GetComponent<ItemBase>().Height,
                        CancelItem.transform.position.z);
                //�I�����Ă����Ɏ����Ă���A�C�e���̎Q�Ƃ�����
                ChangeCurrentHandItem(null);
            }
        }

        //���肪�󂢂Ă���Ƃ�
        if(mLeftItem == null)
        {
            //�A�C�e��UI�̍X�V
            Image _image = mLeftItemBox.transform.GetChild(0).GetComponent<Image>();
            _image.sprite = null;
            _image.color = new Color(1,1,1,0);
            if(mLeftItemPowerBar.IsActive())
            {
                mLeftItemPowerBar.gameObject.SetActive(false);
            }
        }
        //�󂢂Ă��Ȃ��Ƃ�
        else
        {
            //�A�C�e��UI�̍X�V
            ItemBase item = mLeftItem.GetComponent<ItemBase>();
            if (!mLeftItemPowerBar.IsActive())
            {
                mLeftItemPowerBar.gameObject.SetActive(true);
            }
            mLeftItemPowerBar.value = item.AwakePow / item.OriginalAwakePow;
        }

        //�E�肪�󂢂Ă���Ƃ�
        if (mRightItem == null)
        {
            //�A�C�e��UI�̍X�V
            Image _image = mRightItemBox.transform.GetChild(0).GetComponent<Image>();
            _image.sprite = null;
            _image.color = new Color(1, 1, 1, 0);
            if (mRightItemPowerBar.IsActive())
            {
                mRightItemPowerBar.gameObject.SetActive(false);
            }
        }
        //�󂢂Ă��Ȃ��Ƃ�
        else
        {
            //�A�C�e��UI�̍X�V
            ItemBase item = mRightItem.GetComponent<ItemBase>();
            if (!mRightItemPowerBar.IsActive())
            {
                mRightItemPowerBar.gameObject.SetActive(true);
            }
            mRightItemPowerBar.value = item.AwakePow / item.OriginalAwakePow;
        }

        //�I�����Ă�����\���{�b�N�X���n�C���C�g
        if (mCurrentHand == Hands.left)
        {
            mLeftItemBox.GetComponent<Outline>().enabled = true;
            mRightItemBox.GetComponent<Outline>().enabled = false;
        }
        else
        {
            mLeftItemBox.GetComponent<Outline>().enabled = false;
            mRightItemBox.GetComponent<Outline>().enabled = true;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    void ItemPickUp()
    {
        //�v���C���[��e�ɂ���
        mClosestItem.transform.parent = transform;
        //�ʒu����
        mClosestItem.transform.localPosition = new Vector3(0, 0, -2.0f);
        //�摜��\��
        mClosestItem.GetComponentInChildren<SpriteRenderer>().enabled = false;
        mClosestItem.GetComponent<BoxCollider>().enabled = false;
        //�Q�Ƃ�����
        mClosestItem = null;
    }

    /// <summary>
    ///���I�����鏈��
    /// </summary>
    void HandSelect()
    {
        //�A�C�e���X���b�g�̃{�^������������{�^���������ꂽ��A�C�e���{�b�N�X��؂�ւ�
        if((Input.GetButtonDown("ItemSlot") && mCurrentHand == Hands.right)
            ||(Input.GetButtonDown("ItemSlot") && mCurrentHand == Hands.left))
        {
            SwitchHand();
        }
        else if(Input.GetButtonDown("LeftItem"))
        {
            mCurrentHand = Hands.left;
        }
        else if (Input.GetButtonDown("RightItem"))
        {
            mCurrentHand = Hands.right;
        }
    }


    private void OnTriggerStay(Collider other)
    {
        //�A�C�e���ƐڐG
        if (other.gameObject.CompareTag("Item"))
        {
            //��ԋ߂��A�C�e��������
            if(mClosestItem == null)
            {
                mClosestItem = other.gameObject;
            }
            else if(mClosestItem != null &&
                (Vector3.Distance(transform.position, other.transform.position)
                < Vector3.Distance(transform.position, mClosestItem.transform.position))            )
            {
                mClosestItem = other.gameObject;
            }
        }

        //�a���ɓ���������
        if(other.gameObject.CompareTag("Target") && transform.position.x < -14.0f)
        {
            bIsNearTarget = true;
        }

        //�C���^�[�A�N�g�\�A�C�e���ƐڐG����
        if (other.gameObject.CompareTag("Interactable"))
        {
            bIsNearInteractableObject = true;
        }
    }

    private void OnTriggerExit(Collider other)
    {
        //�A�C�e�����痣����
        if (other.gameObject.CompareTag("Item"))
        {
            //��ԋ߂��A�C�e���ł���ΎQ�Ƃ�����
            if(mClosestItem = other.gameObject)
            {
                mClosestItem = null;
            }
        }

        //�a������o�Ă�������
        if (other.gameObject.CompareTag("Target"))
        {
            bIsNearTarget = false;
        }

        //�C���^�[�A�N�g�\�A�C�e�����痣����
        if (other.gameObject.CompareTag("Interactable"))
        {
            bIsNearInteractableObject = false;
        }
    }

    /// <summary>
    /// �I�����Ă���肪�󂢂Ă��邩�̔���
    /// </summary>
    /// <returns>�I�����Ă���肪�󂢂Ă��邩�ǂ���</returns>
    bool IsSelectedHandIsFree()
    {
        switch(mCurrentHand)
        {
            case Hands.left:
            if (mLeftItem == null)
                {
                    return true;
                }
            break;    
            
            case Hands.right:
                if (mRightItem == null)
                {
                    return true;
                }
            break;
        }

        return false;
    }

    /// <summary>
    /// �ʂ̎肪�󂢂Ă��邩�̔���
    /// </summary>
    /// <returns>�ʂ̎肪�󂢂Ă��邩�ǂ���</returns>
    bool IsOtherHandIsFree()
    {
        switch (mCurrentHand)
        {
            case Hands.left:
                if (mRightItem == null)
                {
                    return true;
                }
                break;

            case Hands.right:
                if (mLeftItem == null)
                {
                    return true;
                }
                break;
        }

        return false;
    }

    /// <summary>
    /// �I�����Ă����Ɏ����Ă���A�C�e����؂�ւ�����
    /// </summary>
    /// <param name="obj">�؂�ւ������A�C�e��</param>
    void ChangeCurrentHandItem(GameObject obj)
    {
        switch (mCurrentHand)
        {
            case Hands.left:
                mLeftItem = obj;
                if (obj != null)
                {
                     Image _image = mLeftItemBox.transform.GetChild(0).GetComponent<Image>();
                    _image.sprite = 
                        obj.GetComponentInChildren<SpriteRenderer>().sprite;
                    _image.color = new Color(1, 1, 1, 1);
                }
                    
                break;

            case Hands.right:
                mRightItem = obj;
                if (obj != null)
                {
                    Image _image = mRightItemBox.transform.GetChild(0).GetComponent<Image>();
                    _image.sprite =
                        obj.GetComponentInChildren<SpriteRenderer>().sprite;
                    _image.color = new Color(1, 1, 1, 1);
                }
                break;
        }
    }

    /// <summary>
    /// �ʂ̎�Ɏ����Ă���A�C�e����؂�ւ�����
    /// </summary>
    /// <param name="obj">�؂�ւ������A�C�e��</param>
    void ChangeOtherHandItem(GameObject obj)
    {
        switch (mCurrentHand)
        {
            case Hands.right:
                mLeftItem = obj;
                if (obj != null)
                {
                    Image _image = mLeftItemBox.transform.GetChild(0).GetComponent<Image>();
                    _image.sprite =
                        obj.GetComponentInChildren<SpriteRenderer>().sprite;
                    _image.color = new Color(1, 1, 1, 1);
                }

                break;

            case Hands.left:
                mRightItem = obj;
                if (obj != null)
                {
                    Image _image = mRightItemBox.transform.GetChild(0).GetComponent<Image>();
                    _image.sprite =
                        obj.GetComponentInChildren<SpriteRenderer>().sprite;
                    _image.color = new Color(1, 1, 1, 1);
                }
                break;
        }
    }

    /// <summary>
    /// �I�����Ă���肪�����Ă���A�C�e�����擾
    /// </summary>
    /// <returns>�I�����Ă���肪�����Ă���A�C�e��</returns>
    GameObject GetCurrentHandItem()
    {
        switch (mCurrentHand)
        {
            case Hands.left:             
            return mLeftItem;

            case Hands.right:               
            return mRightItem;

            default:
                return null;
        }
    }

    /// <summary>
    /// �ʂ̎肪�����Ă���A�C�e�����擾
    /// </summary>
    /// <returns>�ʂ̎肪�����Ă���A�C�e��</returns>
    GameObject GetOtherHandItem()
    {
        switch (mCurrentHand)
        {
            case Hands.left:
                return mRightItem;

            case Hands.right:
                return mLeftItem;

            default:
                return null;
        }
    }

    /// <summary>
    /// �I�����Ă�����؂�ւ�
    /// </summary>
    void SwitchHand()
    {
        switch (mCurrentHand)
        {
            case Hands.left:
                mCurrentHand = Hands.right;
                break;

            case Hands.right:
                mCurrentHand = Hands.left;
                break;

            default:
                break;
        }
    }

    /// <summary>
    /// �f�o�b�O�p
    /// </summary>
    void OnGUI()
    {
        if (Application.isEditor)  // or check the app debug flag
        {
            GUI.Label(new Rect(10, 10, 1000, 2000), "Hand:" + mCurrentHand.ToString());
        }
    }
}
