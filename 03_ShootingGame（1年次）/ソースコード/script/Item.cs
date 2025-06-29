// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Item.cs
// プレイヤーが取得するアイテム（パワーアップなど）の基本クラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Item : MonoBehaviour
{
    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        
    }

    // 画面外（ActiveArea）に出たら自身を破棄する
    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "ActiveArea")
        {
            Destroy(gameObject);
        }
    }
}