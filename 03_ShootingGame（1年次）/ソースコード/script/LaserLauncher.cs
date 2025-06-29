// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// LaserLauncher.cs
// レーザー武器の発射エフェクト（パーティクル）を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LaserLauncher : MonoBehaviour
{
    public GameObject laser; // レーザー本体のプレハブ
    ParticleSystem ps; // パーティクルシステム
    ParticleSystem.ColorOverLifetimeModule pscm; // パーティクルの色（時間経過）モジュール
    [SerializeField] Gradient[] _gradient; // プレイヤーのモードに応じた色のグラデーション配列
    Player player; // プレイヤーコンポーネントへの参照

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        ps = GetComponent<ParticleSystem>();
        pscm = ps.colorOverLifetime; // 色モジュールを取得
        
        // レーザー本体を生成
        if (laser != null)
        {
            Instantiate(laser, transform.position, Quaternion.identity);
        }

        // 親オブジェクトからPlayerコンポーネントを取得
        player = GetComponentInParent<Player>();
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        if (player == null) return; // プレイヤーがいない場合は処理中断

        // プレイヤーの現在のモードに応じてパーティクルの色を設定
        if (_gradient.Length > player.mode)
        {
            pscm.color = new ParticleSystem.MinMaxGradient(_gradient[player.mode]);
        }

        // パーティクルの放射モジュールを取得
        var emission = ps.emission;

        // メイン武器発射キー（左クリック）が押されている間
        if (Input.GetKey(KeyCode.Mouse0))
        {
            // パーティクルの放射を有効にする
            emission.enabled = true;
        }
        else
        {
            // パーティクルの放射を無効にする
            emission.enabled = false;
        }
    }
}