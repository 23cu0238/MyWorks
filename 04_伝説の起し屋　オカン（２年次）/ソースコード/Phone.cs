//---------------------------------------------------------------------------
//作成者：李朗曦
//作成日時：2024/10/11
//概要：電話アイテムのクラス
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Phone : MonoBehaviour
{
    bool bIsAvalible;       //使えるかどうか
    public bool bIsUsed;    //一度使われたかどうか
    bool bIsActive;         //起動されたかどうか

    GameManager mGameManager;   //ゲームマネージャー

    Material mMat;  //マテリアル

    Animator mAnimator; //アニメーター
    
    // Start is called before the first frame update
    void Start()
    {
        //初期化
        bIsAvalible = false;
        bIsUsed = false;
        bIsActive = false;

        //初期代入
        mGameManager = GameManager.instance;
        mAnimator = GetComponent<Animator>();
        mMat = GetComponent<SpriteRenderer>().material; 
    }

    // Update is called once per frame
    void Update()
    {
        //起動されたかどうかのチェック
        ActivateCheck();
        //アニメションの更新処理
        AnimatorUpdate();

        //起動された＆使えるとき
        if (bIsAvalible && bIsActive)
        {
            //使用チェック（入力待ち）
            InteractionCheck();
            //枠線を表示
            mMat.SetFloat("_OutValue", 0.2f);
        }
        else
        {
            //枠線を非表示
            mMat.SetFloat("_OutValue", 0f);
        }
    }

    /// <summary>
    /// 起動条件チェック
    /// </summary>
    void ActivateCheck()
    {
        //起動条件：迷惑度＞60＆つかわれたことがない
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
    /// アニメションの更新処理
    /// </summary>
    void AnimatorUpdate()
    {
        ///起動中であればアニメション再生
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
    /// 使うボタンが押されたかどうかをチェック
    /// </summary>
    void InteractionCheck()
    {
        if(Input.GetButtonDown("Interaction"))
        {
            //使うと：迷惑度-30、使われたフラグをトゥルー、UIに表示
            mGameManager.InteractWithTarget(0, -30);
            mGameManager.bIsPlayingMinusAnnoyTextAnim = true;
            bIsUsed = true;
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        //プレイヤーと接触すると使えるように
        if(other.gameObject.CompareTag("Player"))
        {
            bIsAvalible = true;
        }
    }

    private void OnTriggerExit(Collider other)
    {
        //プレイヤーから離れると使えないように
        if (other.gameObject.CompareTag("Player"))
        {
            bIsAvalible = false;
        }
    }
}
