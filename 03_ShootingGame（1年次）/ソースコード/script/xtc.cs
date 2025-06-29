// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// xtc.cs
// TextMeshProUGUIのテキストを点滅させるクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class xtc : MonoBehaviour
{
    // 点滅させるテキストコンポーネント
    public TextMeshProUGUI text;

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // 自身のゲームオブジェクトにアタッチされているTextMeshProUGUIを取得
        text = GetComponent<TextMeshProUGUI>();
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // テキストコンポーネントが設定されていれば
        if (text != null)
        {
            // Mathf.PingPongを使って、テキストのアルファ値(透明度)を0と1の間で往復させる
            // これによりテキストが点滅して見える
            text.color = new Color(1, 1, 1, Mathf.PingPong(Time.time, 1));
        }
    }
}