// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// StageClear3ex.cs
// ステージ3クリア後、EXステージへ遷移するための画面を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class StageClear3ex : MonoBehaviour
{
    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // Xキーが押されたらEXステージに遷移する
        if (Input.GetKeyDown(KeyCode.X))
        {
            SceneManager.LoadScene("StageEx");
        }
    }
}