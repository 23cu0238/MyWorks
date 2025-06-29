// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// ProcessBar.cs
// ステージの進行度をUIのSliderで表示するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ProcessBar : MonoBehaviour
{
    ProcessManager pm; // 進行度を管理するProcessManager
    Slider slider; // 進行度表示用のSliderコンポーネント
    
    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // Main CameraオブジェクトからProcessManagerコンポーネントを取得
        pm = GameObject.Find("Main Camera").GetComponent<ProcessManager>();
        // 自分自身のSliderコンポーネントを取得
        slider = GetComponent<Slider>();

        if(pm != null)
        {
            // スライダーの最大値を目標進行度に設定
            slider.maxValue = pm.target;
            // スライダーの初期値を0に設定
            slider.value = 0;
        }
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // ProcessManagerの現在の進行度をスライダーの値に反映
        if (pm != null && slider != null)
        {
            slider.value = pm.process;
        }
    }
}