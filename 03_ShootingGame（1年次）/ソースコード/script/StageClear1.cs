// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// StageClear1.cs
// ステージ1クリア画面の制御を行うクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class StageClear1 : MonoBehaviour
{
    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // 初期化処理が必要な場合はここに追加
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // Xキーが押されたら、ステージ2に遷移する
        if (Input.GetKeyDown(KeyCode.X))
        {
            SceneManager.LoadScene("Stage2");
        }
    }
}