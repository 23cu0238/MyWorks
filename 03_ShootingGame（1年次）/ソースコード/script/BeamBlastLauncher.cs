// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// BeamBlastLauncher.cs
// メイン武器「ビームブラスト」の発射を管理するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BeamBlastLauncher : MonoBehaviour
{
    [SerializeField] GameObject[] BB; // プレイヤーのモードに応じたビームブラストのプレハブ配列
    Player player; // プレイヤーコンポーネントへの参照
    public float cooldown; // クールダウン時間
    public float cdTimer; // 現在のクールダウンタイマー

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // 親オブジェクトからPlayerコンポーネントを取得
        player = GetComponentInParent<Player>();
        cdTimer = 0; // クールダウンタイマーを初期化
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // クールダウンタイマーを減算
        cdTimer -= Time.deltaTime;

        // クールダウンが終了している場合
        if (cdTimer <= 0)
        {
            // メイン武器発射キー（左クリック）が押されている間
            if (Input.GetKey(KeyCode.Mouse0))
            {
                // プレイヤーの現在のモードに応じたビームブラストを生成
                if (player != null && BB.Length > player.mode)
                {
                     Instantiate(BB[player.mode], transform.position, Quaternion.identity);
                     // クールダウンタイマーをリセット
                     cdTimer = cooldown;
                }
            }
        }
    }
}