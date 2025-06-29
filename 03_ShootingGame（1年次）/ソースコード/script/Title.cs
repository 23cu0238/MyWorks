// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Title.cs
// タイトル画面の制御を行うクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Title : MonoBehaviour
{
    // UIとゲームオブジェクトへの参照
    public GameObject Canvas; // 操作説明などを表示するキャンバス
    public GameObject BG; // 背景オブジェクト (現在は未使用)
    private bool showInstruction; // 操作説明が表示されているかどうかのフラグ
    public GameObject ScPrefab; // ScoreSaveオブジェクトのプレハブ
    public GameObject armory; // 武器選択画面のオブジェクト (現在は未使用)

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // シーンにScoreSaveオブジェクトが存在しない場合、プレハブから生成する
        if (GameObject.FindGameObjectWithTag("ScoreSave") == null)
		{
            Instantiate(ScPrefab);
		}
        
        // ScoreSaveオブジェクトを取得し、スコアと残機をリセット
        ScoreSave ss = GameObject.FindGameObjectWithTag("ScoreSave").GetComponent<ScoreSave>();
        ss.clrScore();
        ss.resetLife();
        
        // 操作説明の表示フラグを初期化
        showInstruction = false;
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // Xキーが押された場合
        if (Input.GetKeyDown(KeyCode.X))
        {
            // 操作説明がまだ表示されていなければ
            if (showInstruction == false)
            {
                // キャンバスをアクティブにして操作説明を表示
                Canvas.SetActive(true);
                showInstruction = true;
            }
        }
    }
}