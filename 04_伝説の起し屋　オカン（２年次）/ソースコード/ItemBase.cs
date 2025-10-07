//---------------------------------------------------------------------------
//作成者：李朗曦
//作成日時：2024/10/04
//概要：アイテムの基本クラス
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class ItemBase : MonoBehaviour
{
    [SerializeField] Material mDefaultMat;  //デフォルトのマテリアル
    [SerializeField] Material mOutlineMat;  //枠線に囲まれるマテリアル

    protected GameObject mUICanvas;         //UIカンバス
    [SerializeField] protected GameObject mChallengePrefab; //ミニゲームのプレハブ
    protected GameObject mActiveChallenge;  //アクティブなミニゲーム
    
    Transform mPlayer;  //プレイヤーのトランスフォーム
    
    [SerializeField] protected Vector3 mStartPos;   //初期位置
    
    [SerializeField] protected float mAwakePow;     //覚醒パワー
    public float AwakePow { get {return mAwakePow; } }  //読み取り専用
    
    protected float mOriginalAwakePow;  //パワーの初期値
    public float OriginalAwakePow { get { return mOriginalAwakePow; } } //読み取り専用

    [SerializeField] protected float mAnnoyingPow;  //迷惑パワー
    
    [SerializeField] float mHeight; //地面からの高さ
    public float Height { get => mHeight; }//読み取り専用

    protected float mAwakePowMultiplier;    //覚醒パワーの補正係数
    protected float mAnnoyingPowMultiplier; //迷惑パワーの補正係数
    float mResetTimer;  //リセットする前の残り時間
    [SerializeField] protected float mChallengeAttributes;  //ミニゲームの難易度係数

    [SerializeField] readonly string mComboName; //ベストマッチのアイテム名
    public string ComboName { get => mComboName; }　//読み取り専用
    
    bool IsUsed = false;    //使われたかどうか

    // Start is called before the first frame update
    protected virtual void Start()
    {
        //各変数の初期化と代入
        mUICanvas = GameManager.instance.UICanvas;
        mStartPos = transform.position;
        mOriginalAwakePow = mAwakePow;
        mAwakePowMultiplier = mAnnoyingPowMultiplier = 1.0f;
    }

    // Update is called once per frame
    protected virtual void Update()
    {
        //枠線の更新処理
        UpdateOutline();

        //プレイヤーに拾われたら
        if (transform.parent != null)
        {
            mResetTimer = 0;
            GetComponentInChildren<SpriteRenderer>().color = new Color(1, 1, 1, 1);
        }
        
        //プレイヤーに捨てられたら
        else if (transform.parent == null && IsUsed) Reset();
    }

    /// <summary>
    /// 枠線の更新処理
    /// </summary>
    void UpdateOutline()
    {
        //エラー防止
        if(mPlayer == null) return;
        
        //もし自身はプレイヤーの最も近いアイテムであれば枠線を表示
        if (mPlayer.GetComponent<PlayerController>().mClosestItem == gameObject)
        {
            gameObject.GetComponentInChildren<SpriteRenderer>().material = mOutlineMat;
        }
        else
        {
            gameObject.GetComponentInChildren<SpriteRenderer>().material = mDefaultMat;
        }

        //拾われたら枠線を強制消す
        if (GetComponent<BoxCollider>().enabled == false)
        {
            gameObject.GetComponentInChildren<SpriteRenderer>().material = mDefaultMat;
        }
    }


    private void OnTriggerStay(Collider other)
    {
        //プレイヤーと接触する時相手の参照を登録
        if (other.gameObject.CompareTag("Player"))
        {
            mPlayer = other.gameObject.transform;
        }
    }

    /// <summary>
    /// リセット処理
    /// </summary>
    private void Reset()
    {
        //リセットするまでの時間を計る
        mResetTimer += Time.deltaTime;
        
        //5秒経過
        if(mResetTimer > 5.0f)
        {
            //画像が点滅
            GetComponentInChildren<SpriteRenderer>().color = new Color(1, 1, 1, Mathf.PingPong(Time.time, 1.0f));
        }

        //10秒経過したらリセット
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
    /// アイテムを使うとき呼び出す
    /// </summary>
    /// <param name="_isCombo">持っているアイテムはベストマッチかどうか</param>
    public virtual void Interact(bool _isCombo)
    {
         IsUsed = true;
    }

    /// <summary>
    /// ミニゲームの結果をコールバック
    /// </summary>
    /// <param name="result">結果のenum</param>
    public virtual void CallResult(ChallengesBase.ChallengeResult result)
    {
        //パワーを半減
        mAwakePow = mAwakePow * 0.5f;
    }
}
