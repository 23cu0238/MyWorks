//---------------------------------------------------------------------------
//作成者：李朗曦
//作成日時：2024/10/04
//概要：ゲームの流れを管理するマネージャー
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System;

public sealed class GameManager : MonoBehaviour
{
    private static GameManager _Instance; //シングルトン用インスタンス

    public static GameManager instance　//インスタンス取得用
    {
        get
        {
            if (_Instance == null)
            {
                _Instance = GameObject.FindObjectOfType<GameManager>();
            }

            return _Instance;
        }
    }

    public enum GameStatus　//ゲームの状態を定義するENUM
    {
        idle,       //待機
        playing,    //プレイ中
        pause,      //一時停止中
        challenge,  //ミニゲーム中
        clear,      //クリア
        over,       //ゲームオーバー
    }

    [SerializeField] GameStatus mGameStatus;    //ゲームの状態
    public GameStatus gameStatus { get => mGameStatus; }    //ゲームの状態を取得用

    [SerializeField] GameObject mUICanvas;      //メインUIのカンバスゲームオブジェクト
    public GameObject UICanvas { get => mUICanvas; }        //メインUIのカンバスを取得用
    [SerializeField] GameObject mDigitalClockNumbers;       //デジタル時計の文字オブジェクト
    [SerializeField] GameObject mPunishText;                //迷惑度が満タンになる時出てくる文字オブジェクト
    
    [SerializeField] GameObject mMinusAnnoyingText;         //電話で隣人対応したら、迷惑ゲージを減らした時出てくる文字オブジェクト
    [SerializeField] GameObject mPhone;                     //電話のゲームオブジェクト
    
    [SerializeField] Text mControlTutorialText;
    
    Slider mAwakeGauge;     //覚醒度を表すゲージUI
    Slider mAnnoyingGauge;  //迷惑度を表すゲージUI
    
    [SerializeField] float mAwakeValue;     //覚醒度
    [SerializeField] float mAnnoyingValue;  //迷惑度
    public float AnnoyingValue { get => mAnnoyingValue; }   //迷惑度取得用
    [SerializeField] float mTime;                           //経過した時間
    [SerializeField] float mTimeLimit;                      //時間制限

    [SerializeField] Camera mUICamera;      //UIだけレンダーするカメラ
    [SerializeField] Camera mMainCamera;    //メインカメラ
    [SerializeField] Camera mMapCamera;     //壁描画用カメラ
    [SerializeField] Camera mBGCamera;      //背景だけレンダーするカメラ

    [SerializeField] Image mTimerRing;          //電子時計の周りのUI
    [SerializeField] Image mTimerRingBGCircle;  //そのUIの背景

    bool mIsAnnoyed = false;                    //迷惑度満タンしているかどうか
    public bool bIsPlayingMinusAnnoyTextAnim;   //迷惑度満タンの文字が表示されているかどうか
    public bool bIsResultTextAnimFinished { get; set; } //リザルトのテキストのアニメションが再生完了したかどうか

    // Start is called before the first frame update
    void Start()
    {
        //各カメラの描画順を初期化
        mBGCamera.depth = -2;
        mMapCamera.depth = -1;
        mMainCamera.depth = 0;
        mUICamera.depth = 1;


        //各変数の初期化と代入
        mAwakeValue = 0;
        mAnnoyingValue = 0;
        mTime = 0;
        mAwakeGauge = mUICanvas.transform.GetChild(1).GetComponent<Slider>();
        mAwakeGauge.maxValue = 100.0f;

        mAnnoyingGauge = mUICanvas.transform.GetChild(2).GetComponent<Slider>();
        mAnnoyingGauge.maxValue = 100.0f;

        bIsResultTextAnimFinished = false;

        //リザルト画面がまだ存在していればそれを消す
        if (SceneManager.GetSceneByName("Result").isLoaded)
        {
            SceneManager.UnloadSceneAsync("Result");
        }
    }

    // Update is called once per frame
    void Update()
    {
        //ゲームの状態によって分岐処理
        switch (mGameStatus)
        {
            //プレイ中
            case GameStatus.playing:

                //時間を測る
                mTime += Time.deltaTime;

                //迷惑度が少しずつ下がる
                if (mAnnoyingValue > 0)
                {
                    mAnnoyingValue -= Time.deltaTime * 0.5f;
                    Mathf.Clamp(mAnnoyingValue, 0, 100.0f);
                }

                //迷惑度満タンの時その関数を呼び出す
                if (mAnnoyingValue >= 100.0f)
                {
                    Annoyed();
                }

                //目標達成したかどうかをチェック
                CheckGameStatus();
                //UIの更新
                UIUpdate();
                break;

            case GameStatus.challenge:  //ミニゲーム中
                break;

            case GameStatus.over:       //ゲームオーバー
                GameOver();
                break;

            case GameStatus.clear:      //ゲームクリア
                GameClear();
                break;
        }
    }

    /// <summary>
    /// UIの更新処理
    /// </summary>
    void UIUpdate()
    {
        //覚醒度と迷惑度のゲージを同期
        mAwakeGauge.value = mAwakeValue;
        mAnnoyingGauge.value = mAnnoyingValue;

        //時計の更新処理
        {
            string hourNum = "07";  //時間の数字
            if (mGameStatus == GameStatus.over)
            {
                hourNum = "08";
            }

            //コロンの点滅
            if (mTime % 1f >= 0.5f)
            {
                mDigitalClockNumbers.transform.GetChild(0).gameObject.SetActive(false);
            }
            else
            {
                mDigitalClockNumbers.transform.GetChild(0).gameObject.SetActive(true);
            }

            //分の数字に変換（リアルの３秒＝ゲーム内の1分）
            string minuteNum = (MathF.Floor(mTime / 3.0f)).ToString();
            if (int.Parse(minuteNum) < 10)
            {
                minuteNum = "0" + minuteNum;
            }

            //表示する
            mDigitalClockNumbers.GetComponent<Text>().text = hourNum + " " + minuteNum;

            //時計の周りの丸いゲージ
            mTimerRing.fillAmount = 1.0f - (mTime / mTimeLimit);

            //時計の後ろの背景は残り時間30秒以下になったら黄色いに点滅
            if (mTimeLimit - mTime < 30.0f)
            {
                if (Mathf.Ceil(mTime) % 2 == 0)
                {
                    mTimerRingBGCircle.color = Color.yellow;
                }
                else
                {
                    mTimerRingBGCircle.color = Color.white;
                }
            }
        }

        //迷惑度満タンした時の処理
        if (mIsAnnoyed)
        {
            //「-30s」の文字の移動
            mPunishText.transform.localPosition = new Vector3(mPunishText.transform.localPosition.x - 1920.0f * Time.deltaTime, 0, 0);
            //画面の端に到達したらリセット
            if (mPunishText.transform.localPosition.x <= -1460.0f)
            {
                mPunishText.transform.localPosition = new Vector3(1460.0f, 0, 0);
                mIsAnnoyed = false;
            }
        }

        //電話を出た後の処理
        if (bIsPlayingMinusAnnoyTextAnim)
        {
            //「迷惑度-30」の文字の移動
            mMinusAnnoyingText.transform.localPosition = new Vector3(mMinusAnnoyingText.transform.localPosition.x - 1920.0f * Time.deltaTime, 0, 0);
            //画面の端に到達したらリセット
            if (mMinusAnnoyingText.transform.localPosition.x <= -1460.0f)
            {
                mMinusAnnoyingText.transform.localPosition = new Vector3(1460.0f, 0, 0);
                bIsPlayingMinusAnnoyTextAnim = false;
            }
        }

        //操作方法の説明UIの変換
        //コントローラと接続したらコントローラのアイコンに変換
        {
            String[] buttonString = new String[4];
            if (Input.GetJoystickNames()[0] != "")
            {
                buttonString[0] = "Ⓧ";
                buttonString[1] = "Ⓨ";
                buttonString[2] = "Ⓐ";
                buttonString[3] = "Ⓑ";
            }
            else
            {
                buttonString[0] = "E";
                buttonString[1] = "R";
                buttonString[2] = "Q";
                buttonString[3] = "Space";
            }

            //表示
            mControlTutorialText.text = buttonString[0] + "：拾う " + buttonString[1] + "：アイテム選択 " + buttonString[2] + "：捨てる " + buttonString[3] + "：使う";
        }
    }

    /// <summary>
    /// ゲームオーバーの処理
    /// </summary>
    void GameOver()
    {
        //リザルトのシーンを画面上に生成
        if (!SceneManager.GetSceneByName("Result").isLoaded)
            SceneManager.LoadScene("Result", LoadSceneMode.Additive);

        //任意のボタンを押したらプレイシーンのリセットし、リザルト画面を消す
        if (Input.anyKeyDown)
        {
            if (SceneManager.GetSceneByName("Result").isLoaded)
            {
                SceneManager.UnloadSceneAsync("Result");
            }

            SceneManager.LoadSceneAsync("PlayScene");
        }
    }
    
    /// <summary>
    /// ゲームクリアの処理
    /// </summary>
    void GameClear()
    {
        //リザルトのシーンを画面上に生成
        if (!SceneManager.GetSceneByName("Result").isLoaded)
            SceneManager.LoadScene("Result", LoadSceneMode.Additive);
        
        //任意のボタンを押したらプレイシーンのリセットし、リザルト画面を消す
        if (Input.anyKeyDown)
        {
            if (SceneManager.GetSceneByName("Result").isLoaded)
            {
                SceneManager.UnloadSceneAsync("Result");
            }

            SceneManager.LoadSceneAsync("PlayScene");
        }
    }

    /// <summary>
    /// 迷惑度満タンした時の処理
    /// </summary>
    void Annoyed()
    {
        mIsAnnoyed = true;
        //電話を使用可能にする
        mPhone.GetComponent<Phone>().bIsUsed = false;
        //経過時間+30秒
        mTime += 30.0f;
        //迷惑度リセット
        mAnnoyingValue = 0;
    }

    /// <summary>
    /// ゲーム状態をチェック
    /// </summary>
    private void CheckGameStatus()
    {
        if(mGameStatus == GameStatus.playing)
        {
            //覚醒度満タンになったらクリア
            if(mAwakeValue >= 100)
            {
                mGameStatus = GameStatus.clear;
            }
            //時間切れたらゲームオーバー
            else if(mTime >= mTimeLimit)
            {
                mTime = 0;
                mGameStatus=GameStatus.over;
            }
        }
    }

    /// <summary>
    /// アイテムを使った後とミニゲームの結果が出た後の覚醒度と迷惑度の増減
    /// </summary>
    /// <param name="awake">補正する覚醒度</param>
    /// <param name="annoy">補正する迷惑度</param>
    public void InteractWithTarget(float awake, float annoy)
    {
        mAwakeValue += awake;
        mAnnoyingValue += annoy;
    }

    /// <summary>
    /// ミニゲームが始まったことをゲームマネージャーに通知
    /// </summary>
    /// <param name="IsChallenge">ミニゲーム中かどうか</param>
    public void NotifyChallenge(bool IsChallenge)
    {
        if (IsChallenge)
        {
            mGameStatus = GameStatus.challenge;
        }
        else
        {
            mGameStatus = GameStatus.playing;
        }
    }
}

