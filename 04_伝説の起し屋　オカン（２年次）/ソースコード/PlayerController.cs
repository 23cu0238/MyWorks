//---------------------------------------------------------------------------
//作成者：李朗曦
//作成日時：2024/10/04
//概要：プレイヤーを制御するコントローラ
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
    enum Hands　//手の定義
    {
        left,   //左
        right,  //右
    }

    Hands mCurrentHand = Hands.left;    //選択中の手
    
    [SerializeField] float mMoveSpeed;  //移動速度
   
    GameObject mLeftItem = null;    //左手アイテム
    GameObject mRightItem = null;   //右手アイテム
    public GameObject mClosestItem; //最も近いアイテム
    [SerializeField] GameObject mLeftItemBox;   //左手アイテムのUI
    [SerializeField] GameObject mRightItemBox;  //右手アイテムのUI
    [SerializeField] GameObject mBestMatchText; //ベストマッチの文字
    [SerializeField] Slider mLeftItemPowerBar;  //左手アイテムの残るパワーゲージ
    [SerializeField] Slider mRightItemPowerBar; //右手アイテムの残るパワーゲージ
    [SerializeField] Sprite mFrontSprite;       //正面画像
    [SerializeField] Sprite mBackSprite;        //背面画像
    SpriteRenderer mCurrentSprite;              //現在の画像

    GameManager mGameManager;　//ゲームマネージャー

    bool bIsNearTarget = false;     //和室にいるかどうか
    bool bIsItemACombo = false;     //持っているアイテムがベストマッチかどうか
    bool bIsNearInteractableObject; //インターアクト可能なアイテムの近くにいるかどうか

    // Start is called before the first frame update
    void Start()
    {
        //初期代入
        mCurrentSprite = transform.GetChild(0).GetComponent<SpriteRenderer>();
        mGameManager = GameManager.instance;
    }

    // Update is called once per frame
    void Update()
    {
        //プレイ中
        if(mGameManager.gameStatus == GameManager.GameStatus.playing)
        {
            //画像更新処理
            SpriteUpdate();
            //移動処理
            Movement();
            //手を選択する処理
            HandSelect();
            //アイテムの更新処理
            ItemUpdate();
        }

        //プレイ中ではないときは停止
        else 
        {
            GetComponent<Rigidbody>().velocity = Vector3.zero;
        }
    }  

    /// <summary>
    /// 画像の更新処理
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
    /// 移動処理
    /// </summary>
    void Movement()
    {      
        GetComponent<Rigidbody>().velocity = new Vector3(
            Input.GetAxis("Horizontal") * mMoveSpeed * Time.deltaTime
            , 0
            ,Input.GetAxis("Vertical") * mMoveSpeed * Time.deltaTime);   
    }

    /// <summary>
    /// アイテムの更新処理
    /// </summary>
    void ItemUpdate()
    {
        //両手ともアイテムを持っている
        if(mLeftItem != null && mRightItem != null)
        {
            //ベストマッチチェック
            if(mLeftItem.GetComponent<ItemBase>().ComboName == mRightItem.name)
            {
                bIsItemACombo = true;
                mBestMatchText.SetActive(true);
            }
        }

        else　//絶対ベストマッチにならないので
        {
            
            bIsItemACombo = false;
            mBestMatchText.SetActive(false);
        }

        //アイテム拾う処理
        if (Input.GetButtonDown("Pickup"))
        {
            //選択している手が空いている＆最もアイテムが存在
            if (IsSelectedHandIsFree()&& mClosestItem != null)
            {
                //拾う
                ChangeCurrentHandItem(mClosestItem);
                ItemPickUp();
            }
            //選択している手が空いていない＆最もアイテムが存在＆別の手が空いている
            else if (!IsSelectedHandIsFree() && mClosestItem != null && IsOtherHandIsFree())
            {
                //拾う
                ChangeOtherHandItem(mClosestItem);
                ItemPickUp();
            }
        }

        //アイテムを使う
        if(Input.GetButtonDown("Interaction"))
        {
            //選択中の手がアイテムを持っている＆和室にいる＆近くにインターアクト可能なアイテムがない
            if (!IsSelectedHandIsFree() && bIsNearTarget && !bIsNearInteractableObject)
            {
                //手にあるのは片手持ちアイテムであれば
                if(GetCurrentHandItem().TryGetComponent<OneHandItem>(out OneHandItem Currentitem))
                {
                    //別の手がアイテムを持っている＆その手にあるのは片手持ちアイテムであれば
                    if (!IsOtherHandIsFree() && GetOtherHandItem().TryGetComponent<OneHandItem>(out OneHandItem Otheritem))
                    {
                        //参照を登録
                        Currentitem.mOtherHandItem = Otheritem;
                    }
                    //そうではなければ
                    else
                    {
                        //参照を消す
                        Currentitem.mOtherHandItem = null;
                    }
                }
                
                //使う関数を呼び出す
                GetCurrentHandItem().GetComponent<ItemBase>().Interact(bIsItemACombo);
                //ゲームマネージャーを通知
                GameManager.instance.NotifyChallenge(true);
            }
        }

        //捨てる処理
        if (Input.GetButtonDown("Cancel"))
        {
            GameObject CancelItem = null;
            //捨てたいアイテムを指定
            if (!IsSelectedHandIsFree())
            {
                CancelItem = GetCurrentHandItem().gameObject;               
            }
            else if (!IsOtherHandIsFree())
            {
                SwitchHand();
                CancelItem = GetCurrentHandItem().gameObject;
                
            }

            //捨てたいアイテムが存在すれば
            if (CancelItem != null)
            {
                //コライダーと画像描画を有効化
                CancelItem.GetComponent<BoxCollider>().enabled = true;
                CancelItem.GetComponentInChildren<SpriteRenderer>().enabled = true;
                //親子関係を解除
                CancelItem.transform.parent = null;
                //位置を調整
                CancelItem.transform.position =
                    new Vector3(CancelItem.transform.position.x,
                        CancelItem.transform.GetComponent<ItemBase>().Height,
                        CancelItem.transform.position.z);
                //選択している手に持っているアイテムの参照を消す
                ChangeCurrentHandItem(null);
            }
        }

        //左手が空いているとき
        if(mLeftItem == null)
        {
            //アイテムUIの更新
            Image _image = mLeftItemBox.transform.GetChild(0).GetComponent<Image>();
            _image.sprite = null;
            _image.color = new Color(1,1,1,0);
            if(mLeftItemPowerBar.IsActive())
            {
                mLeftItemPowerBar.gameObject.SetActive(false);
            }
        }
        //空いていないとき
        else
        {
            //アイテムUIの更新
            ItemBase item = mLeftItem.GetComponent<ItemBase>();
            if (!mLeftItemPowerBar.IsActive())
            {
                mLeftItemPowerBar.gameObject.SetActive(true);
            }
            mLeftItemPowerBar.value = item.AwakePow / item.OriginalAwakePow;
        }

        //右手が空いているとき
        if (mRightItem == null)
        {
            //アイテムUIの更新
            Image _image = mRightItemBox.transform.GetChild(0).GetComponent<Image>();
            _image.sprite = null;
            _image.color = new Color(1, 1, 1, 0);
            if (mRightItemPowerBar.IsActive())
            {
                mRightItemPowerBar.gameObject.SetActive(false);
            }
        }
        //空いていないとき
        else
        {
            //アイテムUIの更新
            ItemBase item = mRightItem.GetComponent<ItemBase>();
            if (!mRightItemPowerBar.IsActive())
            {
                mRightItemPowerBar.gameObject.SetActive(true);
            }
            mRightItemPowerBar.value = item.AwakePow / item.OriginalAwakePow;
        }

        //選択している手を表すボックスをハイライト
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
        //プレイヤーを親にする
        mClosestItem.transform.parent = transform;
        //位置調整
        mClosestItem.transform.localPosition = new Vector3(0, 0, -2.0f);
        //画像非表示
        mClosestItem.GetComponentInChildren<SpriteRenderer>().enabled = false;
        mClosestItem.GetComponent<BoxCollider>().enabled = false;
        //参照を消す
        mClosestItem = null;
    }

    /// <summary>
    ///手を選択する処理
    /// </summary>
    void HandSelect()
    {
        //アイテムスロットのボタンか手を交換ボタンが押されたらアイテムボックスを切り替え
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
        //アイテムと接触
        if (other.gameObject.CompareTag("Item"))
        {
            //一番近いアイテムを決定
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

        //和室に入った判定
        if(other.gameObject.CompareTag("Target") && transform.position.x < -14.0f)
        {
            bIsNearTarget = true;
        }

        //インターアクト可能アイテムと接触判定
        if (other.gameObject.CompareTag("Interactable"))
        {
            bIsNearInteractableObject = true;
        }
    }

    private void OnTriggerExit(Collider other)
    {
        //アイテムから離れると
        if (other.gameObject.CompareTag("Item"))
        {
            //一番近いアイテムであれば参照を消す
            if(mClosestItem = other.gameObject)
            {
                mClosestItem = null;
            }
        }

        //和室から出てきた判定
        if (other.gameObject.CompareTag("Target"))
        {
            bIsNearTarget = false;
        }

        //インターアクト可能アイテムから離れると
        if (other.gameObject.CompareTag("Interactable"))
        {
            bIsNearInteractableObject = false;
        }
    }

    /// <summary>
    /// 選択している手が空いているかの判定
    /// </summary>
    /// <returns>選択している手が空いているかどうか</returns>
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
    /// 別の手が空いているかの判定
    /// </summary>
    /// <returns>別の手が空いているかどうか</returns>
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
    /// 選択している手に持っているアイテムを切り替え処理
    /// </summary>
    /// <param name="obj">切り替えたいアイテム</param>
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
    /// 別の手に持っているアイテムを切り替え処理
    /// </summary>
    /// <param name="obj">切り替えたいアイテム</param>
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
    /// 選択している手が持っているアイテムを取得
    /// </summary>
    /// <returns>選択している手が持っているアイテム</returns>
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
    /// 別の手が持っているアイテムを取得
    /// </summary>
    /// <returns>別の手が持っているアイテム</returns>
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
    /// 選択している手を切り替え
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
    /// デバッグ用
    /// </summary>
    void OnGUI()
    {
        if (Application.isEditor)  // or check the app debug flag
        {
            GUI.Label(new Rect(10, 10, 1000, 2000), "Hand:" + mCurrentHand.ToString());
        }
    }
}
