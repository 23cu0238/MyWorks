// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// HPCanvas.cs
// 敵のHPバーなどを表示するワールド空間UIキャンバスのカメラ設定を行うクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HPCanvas : MonoBehaviour
{
    Canvas canvas; // このスクリプトがアタッチされているCanvasコンポーネント

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        canvas = GetComponent<Canvas>();
        
        // キャンバスのレンダリングモードが "World Space" の場合、
        // イベントを受け取るためのカメラ（Event Camera）を設定する必要がある
        if(canvas != null && canvas.renderMode == RenderMode.WorldSpace)
        {
            // シーンのメインカメラをイベントカメラとして設定
            canvas.worldCamera = Camera.main;
        }
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // 更新処理は特に無し
    }
}