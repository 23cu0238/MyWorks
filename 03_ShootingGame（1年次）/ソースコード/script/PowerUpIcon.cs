// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// PowerUpIcon.cs
// UI上にサブウェポンのアイコンを表示するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PowerUpIcon : MonoBehaviour
{
    SpriteRenderer sr; // アイコン表示用のSpriteRenderer
    public Sprite[] sprite; // サブウェポンごとのスプライト配列
    
    Player player; // プレイヤーコンポーネントへの参照

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player")?.GetComponent<Player>();
        sr = GetComponent<SpriteRenderer>();
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // プレイヤーオブジェクトが存在するか確認
        if (GameObject.FindGameObjectWithTag("Player") != null)
        {
            // プレイヤーコンポーネントを再取得（プレイヤーがリスポーンした場合のため）
            player = GameObject.FindGameObjectWithTag("Player").GetComponent<Player>();
            if (player != null && sprite.Length > player.SubWeaponIndex && player.SubWeaponIndex >= 0)
            {
                // プレイヤーの現在のサブ武器インデックスに応じてスプライトを切り替える
                sr.sprite = sprite[player.SubWeaponIndex];
            }
        }
    }
}