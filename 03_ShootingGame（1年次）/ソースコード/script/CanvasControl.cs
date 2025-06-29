// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// CanvasControl.cs
// UIキャンバス上の要素（ボスHPバーなど）の表示を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CanvasControl : MonoBehaviour
{
    public GameObject BossHP; // ボスのHPバーUIオブジェクトへの参照

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // 初期化処理をここに追加できます
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // 更新処理をここに追加できます
    }

    // ボスHPバーを有効化（表示）する関数
    public void ActivateHPBar()
    {
        if(BossHP != null)
        {
            BossHP.SetActive(true);
        }
    }    
}