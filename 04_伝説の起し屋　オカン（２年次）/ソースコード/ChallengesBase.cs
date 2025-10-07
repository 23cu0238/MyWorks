//---------------------------------------------------------------------------
//作成者：李朗曦
//作成日時：2024/10/11
//概要：ミニゲームオブジェクトの基本クラス
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ChallengesBase : MonoBehaviour
{
    public enum ChallengeResult //ミニゲームの結果のENUM定義
    {
        none,
        great,
        good,
        bad
    }

    protected ChallengeResult mChallengeResult;　//ミニゲームの結果

    [SerializeField] GameObject mResultTextObj;　//ミニゲームの結果を表示する文字オブジェクト
    public GameObject mUsedObj;                  //このミニゲームを生成したアイテムのオブジェクト
    [SerializeField] float mScrollSpeed;         //文字オブジェクトの移動速度
    Text mResultText;                            //ミニゲームの結果を表示する文字

    public float mChallengeAttributes;           //ミニゲームの種類によって色んな役割を持つ値に変換用
    private float mIdleTimer;                    ////文字オブジェクトの移動停止した時間を測る

    // Start is called before the first frame update
    protected virtual void Start()
    {
        //初期化
        mResultText = mResultTextObj.GetComponent<Text>();
        mIdleTimer = 0;
    }

    // Update is called once per frame
    protected virtual void Update()
    {
        //結果が出た時の処理
        if (mChallengeResult != ChallengeResult.none)
        {
            //文字オブジェクトを起動
            mResultTextObj.SetActive(true);
            
            //結果によって文字の分岐処理
            //great:パワー　+50%, 緑
            //good:ボーナスなし,　黄色
            //bad:迷惑+20%,       赤
            switch (mChallengeResult)
            { 
                case ChallengeResult.great:
                    mResultText.text = "Great!\nPower +50%";
                    mResultText.color = Color.green;
                    break;

                case ChallengeResult.good:
                    mResultText.text = "Good!";
                    mResultText.color = Color.yellow;
                    break;

                case ChallengeResult.bad:
                    mResultText.text = "Bad!\nAnnoying +20%";
                    mResultText.color = Color.red;
                    break;

                default:
                    break;
            }
            
            //文字オブジェクトの移動処理
            float newYpos = Mathf.Clamp(mResultTextObj.transform.localPosition.y - mScrollSpeed * Time.deltaTime, 115.0f,1000.0f);
            mResultTextObj.transform.localPosition = new Vector3(mResultTextObj.transform.localPosition.x,
                           newYpos,
                           -10.0f);

            //指定の位置に到達したら
            if(newYpos <= 115.0f)
            {
                //時間計測
                mIdleTimer += Time.deltaTime;
                
                //一秒経過したら
                if (mIdleTimer > 1.0f)
                {
                    //結果をアイテムオブジェクトとゲームマネージャーに通知
                    mUsedObj.GetComponent<ItemBase>().CallResult(mChallengeResult);
                    GameManager.instance.NotifyChallenge(false);
                    Destroy(gameObject);    //自滅
                }
            }
        }

    }
}
