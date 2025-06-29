// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// EMP.cs
// プレイヤーが取得するEMPアイテムの挙動を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EMP : MonoBehaviour
{
    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // 初期化処理
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // 更新処理
    }

    // 画面外に出たら自身を破棄
    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "ActiveArea")
        {
            Destroy(gameObject);
        }
    }
}