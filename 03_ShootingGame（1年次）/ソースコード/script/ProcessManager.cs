// 作成者：日本電子専門学校ゲーム制作研究科 23cu0238李朗曦
// ProcessManager.cs
// ステージの進行度を管理するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProcessManager : MonoBehaviour
{
    public int target; // 目標となる進行度
    public int process; // 現在の進行度

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // processの初期化が必要ならここで行う
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        
    }

    // 進行度を加算する関数
    public void ProcessPlus(int value)
    {
        process += value;
    }
}