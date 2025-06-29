// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// StageClear3.cs
// ステージ3クリア画面の制御を行うクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class StageClear3 : MonoBehaviour
{
    // メンバ変数
    int finalScore; // 最終スコアを格納する変数
    public TextMeshProUGUI ScoreText; // スコアを表示するUIテキスト

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // ScoreSaveオブジェクトから最終スコアを取得
        ScoreSave ss = GameObject.FindGameObjectWithTag("ScoreSave").GetComponent<ScoreSave>();
        finalScore = ss.GetSavedScore();
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // スコアをUIテキストに表示
        ScoreText.text = "Total Score:" + finalScore.ToString();

        // Xキーが押されたらタイトル画面に戻る
        if (Input.GetKeyDown(KeyCode.X))
        {
            SceneManager.LoadScene("Title");
        }
    }
}