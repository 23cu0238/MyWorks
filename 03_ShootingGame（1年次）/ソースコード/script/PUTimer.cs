// 作成者：日本電子専門学校ゲーム制作研究科 23cu0238李朗曦
// PUTimer.cs
// プレイヤーのパワーアップ残り時間をUIスライダーで表示するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PUTimer : MonoBehaviour
{
    Slider PUT; // パワーアップ時間表示用のUIスライダー

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        PUT = GetComponent<Slider>(); // スライダーコンポーネントを取得
        PUT.maxValue = 10; // スライダーの最大値を10秒に設定
        // プレイヤーオブジェクトを探し、そのパワーアップタイマーを初期値として設定
        GameObject playerObj = GameObject.Find("Player");
        if(playerObj != null)
        {
            PUT.value = playerObj.GetComponent<Player>().powerUpTimer;
        }
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // プレイヤーオブジェクトを探す
        GameObject playerObj = GameObject.Find("Player");
        // プレイヤーがいない場合（死亡時など）はタイマーを0にする
        if (playerObj == null)
        {
            PUT.value = 0;
        }
        // プレイヤーがいる場合は、現在のパワーアップタイマーをスライダーに反映
        else if (playerObj != null)
        {
            PUT.value = playerObj.GetComponent<Player>().powerUpTimer;
        }
    }
}