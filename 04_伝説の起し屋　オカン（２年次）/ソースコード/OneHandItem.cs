//---------------------------------------------------------------------------
//作成者：李朗曦
//作成日時：2024/10/04
//概要：片手持ちアイテム派生クラス
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OneHandItem : ItemBase
{
    bool bIsCombo = false; //持っているアイテムはベストマッチかどうか

    public OneHandItem mOtherHandItem;  //別の手に持っているアイテム

    // Start is called before the first frame update
    protected override void Start()
    {
        //初期化
        mOtherHandItem = null;
        //基本クラスのスタート処理
        base.Start();
    }

    // Update is called once per frame
    protected override void Update()
    {
        //基本クラスの更新処理
        base.Update();
    }

    /// <summary>
    /// 片手持ちアイテムを使うとき呼び出す
    /// </summary>
    /// <param name="_isCombo">持っているアイテムはベストマッチかどうか</param>
    public override void Interact(bool _isCombo)
    {
        //基本クラスの関数を呼び出す
        base.Interact(bIsCombo);    

        //起動中のミニゲームが存在しなければ生成する
        if(mActiveChallenge == null)
        {
            mActiveChallenge = Instantiate(mChallengePrefab, mUICanvas.transform);
        }

        //難易度補正係数をセットする
        mActiveChallenge.GetComponent<ChallengesBase>().mChallengeAttributes = mChallengeAttributes;
        //使用済フラグをセットする
        mActiveChallenge.GetComponent<ChallengesBase>().mUsedObj = gameObject;

        //ベストマッチかどうかをメンバー変数に代入
        bIsCombo = _isCombo;
    }

    /// <summary>
    /// ミニゲームの結果をコールバック
    /// </summary>
    /// <param name="result">結果のenum</param>
    public override void CallResult(ChallengesBase.ChallengeResult result)
    {
        //もし別の手もアイテムを持っていればそちも呼び出す
        if (mOtherHandItem != null)
        {
            mOtherHandItem.CallResult(result);
        }

        //アクティブなミニゲームの参照を消す
        if (mActiveChallenge != null)
        {
            mActiveChallenge = null;
        }

        //リザルトに応じて処理
        switch(result)
        {
            case ChallengesBase.ChallengeResult.great:
                mAwakePowMultiplier += 0.5f;    //覚醒補正係数+50％
                break;
            case ChallengesBase.ChallengeResult.good:
                break;
            case ChallengesBase.ChallengeResult.bad:
                mAnnoyingPowMultiplier += 0.2f; //迷惑補正係数+20％
                break;
        }

        //ベストマッチであれば
        if (bIsCombo)
        {
            mAwakePowMultiplier += 0.5f;　//覚醒補正係数+50％
        }
        
        //ゲームマネージャー側の処理
        GameManager.instance.InteractWithTarget(mAwakePow * mAwakePowMultiplier, mAnnoyingPow * mAnnoyingPowMultiplier);
        
        //各補正係数をリセット
        mAwakePowMultiplier = mAnnoyingPowMultiplier = 1.0f;
        //別の手の参照を消す
        mOtherHandItem = null;
        //基本クラスの関数を呼び出す
        base.CallResult(result);
    }
}
