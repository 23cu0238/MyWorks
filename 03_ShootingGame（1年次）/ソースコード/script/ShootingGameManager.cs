// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// ShootingGameManager.cs
// ゲーム全体の進行、スコア、残機、プレイヤーのスポーンを管理するマネージャークラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class ShootingGameManager : MonoBehaviour
{
    // メンバ変数
    float gameOverTime = 3; // プレイヤーの残機が0の時にゲームオーバーになるまでの時間
    float playerDeadTimer; // プレイヤーがやられてからの経過時間を記録するタイマー
    float respawnTime = 2; // プレイヤーがやられてからリスポーンするまでの待機時間
    float deadTimer; // プレイヤーがいない状態が続いてからの経過時間タイマー
    Player.Status playerStatus; // プレイヤーの状態を管理 (Fine:生存, Dead:死亡)
    public Text ScoreText; // スコアを表示するUIテキスト
    public Text LifeText; // 残機を表示するUIテキスト
    public int totalScore; // 現在の合計スコア
    public int Life; // 現在の残機
    public GameObject PlayerPrefab; // リスポーンさせるプレイヤーのプレハブ
    static Vector3 spawnPosition; // プレイヤーのリスポーン位置
    float endTimer; // ステージクリア時の演出用タイマー
    bool IsEnd; // ステージクリア演出中かどうかのフラグ

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // 各タイマーとフラグを初期化
        endTimer = 0;
        playerDeadTimer = 0;
        playerStatus = Player.Status.Fine;

        // シーンをまたいでデータを保持するScoreSaveオブジェクトを探す
        if (GameObject.FindGameObjectWithTag("ScoreSave") != null)
        {
            // ScoreSaveオブジェクトからスコアと残機を読み込む
            ScoreSave ss = GameObject.FindGameObjectWithTag("ScoreSave").GetComponent<ScoreSave>();
            totalScore = ss.GetSavedScore();
            Life = ss.GetSavedLife();
        }

        // UIテキストを初期化
        ScoreText.text = "Total Score:" + totalScore;
        LifeText.text = "Life left:"+Life;
        // リスポーン位置を設定
        spawnPosition = new Vector3(0.0f, -3.5f, 0.0f);
        deadTimer = 0;
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // ステージクリア演出中の処理
        if (IsEnd==true)
        {
            endTimer += Time.deltaTime; // タイマーを進める
            // 2秒経過したら、現在のステージ名に応じて次のシーンに遷移
            if(endTimer>=2)
            {
                if (SceneManager.GetActiveScene().name == "Stage1")
                {
                    SceneManager.LoadScene("Stage1_Clear");
                }
                else if (SceneManager.GetActiveScene().name == "Stage2")
                {
                    SceneManager.LoadScene("Stage2_Clear");
                }
                else if (SceneManager.GetActiveScene().name == "Stage3")
                {
                    SceneManager.LoadScene("Stage3_ExClear");
                }
            }    
        }    

        // プレイヤーが完全に死亡した状態（残機0で破壊された後）の処理
        if (playerStatus==Player.Status.Dead)
        {
            playerDeadTimer += Time.deltaTime; // タイマーを進める
            
            // ゲームオーバー待機時間を超えたらゲームオーバーシーンに遷移
            if(playerDeadTimer>= gameOverTime)
            {
                SceneManager.LoadScene("GameOver");
                playerDeadTimer = 0.0f; // タイマーリセット
            }
        }

        // プレイヤーオブジェクトが存在せず、かつ残機も0の場合
        if (GameObject.FindGameObjectsWithTag("Player").Length < 1 && Life <= 0)
        {
            // プレイヤーの状態を「死亡」に設定
            playerStatus = Player.Status.Dead;
        }
        // プレイヤーオブジェクトは存在しないが、残機がまだある場合
        else if (GameObject.FindGameObjectsWithTag("Player").Length < 1 && Life > 0)
        {
            // リスポーン待機タイマーを進める
            deadTimer += Time.deltaTime;
        }

        // リスポーン待機時間を超えたら
        if( deadTimer >= respawnTime)
        {
            // プレイヤーをリスポーンさせ、残機を1減らす
            Instantiate(PlayerPrefab, spawnPosition, Quaternion.identity);
            Life--;
            deadTimer = 0; // タイマーリセット
        }

        // 残機表示を更新
        LifeText.text = "Life left:" + Life;
    }

   
    // スコアを加算する関数
    public void AddScore(int score)
    {
        totalScore += score;
        ScoreText.text = "Total Score:"+totalScore.ToString(); // UIを更新
    }

    // 現在のスコアを取得する関数
    public int GetScore()
    {
        return totalScore;
    }

    // 現在の情報をScoreSaveオブジェクトに保存する関数
    public void SaveInfo()
    {
        ScoreSave sc=GameObject.FindGameObjectWithTag("ScoreSave").GetComponent<ScoreSave>();
        sc.SaveScore(totalScore);
        sc.SaveLife(Life);
    }

    // スコア表示を強制的に更新する関数
    public void renewScore()
    {
        ScoreText.text = "Total Score:" + totalScore.ToString();
    }
    
    // 残機をリセットする関数
    public void resetLife()
    {
        Life = 3;
    }

    // 残機を1増やす関数
    public void AddLife()
    {
        Life+=1;
    }

    // ステージクリア演出を開始するための関数
    public void LoadClear()
    {
        IsEnd = true;
    }
}