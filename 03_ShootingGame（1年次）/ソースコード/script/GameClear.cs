// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// GameClear.cs
// ゲームクリア画面でタイトルへの復帰を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameClear : MonoBehaviour
{
    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {

    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // Xキーが押されたらタイトルシーンに遷移
        if (Input.GetKeyDown(KeyCode.X))
        {
            SceneManager.LoadScene("Title");
        }
    }
}