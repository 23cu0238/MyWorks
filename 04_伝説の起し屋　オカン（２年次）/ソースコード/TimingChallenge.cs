//---------------------------------------------------------------------------
//作成者：李朗曦
//作成日時：2024/10/11
//概要：タイミングのミニゲーム派生クラス
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TimingChallenge : ChallengesBase
{
    Scrollbar mScrollbar;                   //判定ゲージ
    [SerializeField] float mGreatZoneRange; //greatの判定範囲
    [SerializeField] float mGoodZoneRange;  //goodの判定範囲

    // Start is called before the first frame update
    protected override void Start()
    {
        //基本クラスのスタート処理
        base.Start();
        //代入
        mScrollbar = transform.GetChild(2).GetComponent<Scrollbar>();
    }

    // Update is called once per frame
    protected override void Update()
    {
        //結果が出ていない時の処理
        if (mChallengeResult == ChallengeResult.none)
        {
            //カーソルを移動させる
            mScrollbar.value = Mathf.PingPong(Time.time * mChallengeAttributes, 1.0f);
            //使うボタンが押されたら
            if (Input.GetButtonDown("Interaction"))
            {
                //結果判定
                if (mScrollbar.value > 0.5f - mGreatZoneRange / 2.0f && mScrollbar.value < 0.5f + mGreatZoneRange / 2.0f)
                {
                    mChallengeResult = ChallengeResult.great;
                }
                else if (mScrollbar.value > 0.5f - mGoodZoneRange / 2.0f && mScrollbar.value < 0.5f + mGoodZoneRange / 2.0f)
                {
                    mChallengeResult = ChallengeResult.good;
                }
                else
                {
                    mChallengeResult = ChallengeResult.bad;
                }
            }
        }
        //基本クラスの更新処理
        base.Update();
    }
}
