//---------------------------------------------------------------------------
//作成者：李朗曦
//作成日時：2024/10/11
//概要：リザルトシーンを制御するマネージャー
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;



public class ResultSceneManager : MonoBehaviour
{
    [SerializeField] Animator mTextAnimator;        //テキストのアニメーター
    [SerializeField] Text mText;                    //テキストのコンポーネント
    [SerializeField] GameObject mClearIllustration; //クリアのカットイン画像オブジェクト

    // Start is called before the first frame update
    void Start()
    {
        //アニメション再生し、テキスト変換
        mTextAnimator.Play("Start",0);
        mText.text = ("終");
    }

    // Update is called once per frame
    void Update()
    {
        //アニメションが一定的な時間以上再生されたら
        if (mTextAnimator.GetCurrentAnimatorStateInfo(0).normalizedTime >= 0.6)
        {
            //クリアの場合
            if(GameManager.instance.gameStatus == GameManager.GameStatus.clear)
            {
                mText.gameObject.SetActive(false);
                mClearIllustration.SetActive(true);
            }
            //ゲームオーバーの場合
            else
            {
                mText.text = "失敗";
            }
        }

        //再生完了
        if (mTextAnimator.GetCurrentAnimatorStateInfo(0).normalizedTime >= 1)
        {
            //ゲームマネージャーに通知
            GameManager.instance.bIsResultTextAnimFinished = true;
        }
    }
}
