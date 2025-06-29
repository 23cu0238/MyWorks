// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// CD.cs
// 武器などのクールダウン情報を保持するためのシンプルなデータクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CD : MonoBehaviour
{
    public float CooldownAmount; // スキルの総クールダウン時間
    public float cd; // 現在の残りクールダウン時間

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // 初期化処理
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // このスクリプトは主に他のスクリプトからデータを参照されるために使われます
    }
}