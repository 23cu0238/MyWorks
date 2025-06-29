// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// BGScroll.cs
// 背景を縦にスクロールさせるクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BGScroll : MonoBehaviour
{
    public float scrollSpeed; // スクロール速度
    private Vector3 startPosition; // 初期位置

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // オブジェクトの初期位置を保存
        startPosition = transform.position;
    }

    // 毎フレーム呼ばれます
    void Update()
    {        
        // Mathf.Repeat を使って、値が0から10の間をループするように新しいY座標を計算
        // これにより、背景が一定の範囲を繰り返しスクロールするようになります
        float newPosition = Mathf.Repeat(Time.time * scrollSpeed, 20.48f); // ループの長さを調整 (例: 20.48)
        
        // 初期位置から計算された新しいY座標分だけ下に移動させる
        transform.position = startPosition + Vector3.down * newPosition;
    }
}