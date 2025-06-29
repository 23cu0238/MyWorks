// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// control.cs
// タイトル画面などで操作説明の表示を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class control : MonoBehaviour
{
    public GameObject Canvas; // 表示/非表示を切り替えるCanvasオブジェクト
    Camera MC; // メインカメラ

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        MC = Camera.main; 
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // Xキーが押されたら
        if (Input.GetKeyDown(KeyCode.X))
        {
            // Canvasをアクティブにし、このオブジェクト自身を破棄する
            if (Canvas != null)
            {
                Canvas.SetActive(true);
            }
            Destroy(gameObject);
        }
    }
}