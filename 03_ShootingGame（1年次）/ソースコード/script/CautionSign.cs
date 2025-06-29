// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// CautionSign.cs
// ボス出現前の警告サインの表示と点滅を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CautionSign : MonoBehaviour
{
    SpriteRenderer sr; // スプライト描画用コンポーネント
    float timer; // 表示時間を管理するタイマー

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        sr = GetComponent<SpriteRenderer>();
        timer = 0;
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        timer += Time.deltaTime;

        // PingPong関数を使ってアルファ値（透明度）を0と1の間で往復させ、点滅させる
        sr.color = new Color(1, 1, 1, Mathf.PingPong(Time.time * 2, 1)); // 速度を調整

        // 3秒経過したら自身を破棄
        if (timer >= 3)
        {
            Destroy(gameObject);
        }
    }
}