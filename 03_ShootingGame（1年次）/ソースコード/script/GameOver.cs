// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// GameOver.cs
// ゲームオーバー画面で最終スコア表示とタイトル復帰を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class GameOver : MonoBehaviour
{
    int finalScore; // 最終スコア
    public TextMeshProUGUI ScoreText; // スコア表示用のUIテキスト

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // シーンをまたいでデータを保持しているScoreSaveオブジェクトを探す
        GameObject scoreSaveObject = GameObject.FindGameObjectWithTag("ScoreSave");
        if (scoreSaveObject != null)
        {
            // ScoreSaveコンポーネントから保存されているスコアを取得
            ScoreSave ss = scoreSaveObject.GetComponent<ScoreSave>();
            finalScore = ss.GetSavedScore();
        }
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // UIテキストに最終スコアと操作案内を表示
        ScoreText.text = "Total Score:" + finalScore.ToString() + "\nPress 'X' to continue." ;

        // Xキーが押されたらタイトルシーンに遷移
        if (Input.GetKeyDown(KeyCode.X))
        {
            SceneManager.LoadScene("Title");
        }
    }
}