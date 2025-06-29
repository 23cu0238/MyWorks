// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// beamArrow.cs
// プレイヤーのサブ武器「ビームアロー」の挙動を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class beamArrow : MonoBehaviour
{
    public float movespeed; // ビームアローの移動速度
    public int power; // ビームアローの攻撃力
    SpriteRenderer sr; // スプライト描画用のコンポーネント

    // ビームアローの状態を定義する列挙体
    public enum Mode
    {
        idle,   // 待機状態
        active, // 発射後のアクティブ状態
    };

    public Mode mode; // 現在の状態

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // 初期状態を待機に設定
        mode = Mode.idle;
        // SpriteRendererコンポーネントを取得
        sr = GetComponent<SpriteRenderer>();
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // アクティブ状態の場合
        if (mode == Mode.active)
        {
            // 色を赤に設定し、前方に直進させる
            sr.color = new Color(1, 0, 0, 1);
            transform.Translate(0, movespeed, 0);
        }
        // 待機状態の場合
        else if (mode == Mode.idle)
        {
            // PingPong関数を使って色を明滅させる
            sr.color = new Color(Mathf.PingPong(Time.time, 1), 0, 0, 1);
        }
    }

    // 他のコライダーと接触した瞬間に呼ばれます
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // アクティブ状態でのみ当たり判定を行う
        if (mode == Mode.active)
        {
            // 敵またはボスにヒットした場合
            if (collision.gameObject.tag == "Enemy" || collision.gameObject.tag == "Boss")
            { 
                // 相手のHPコンポーネントを取得し、ダメージを与える
                HP hit = collision.gameObject.GetComponent<HP>();
                if (hit != null) // 安全のためnullチェック
                {
                    hit.Damage(power);
                }
            }
        }
    }

    // 他のコライダーがアクティブエリアから出た瞬間に呼ばれます
    private void OnTriggerExit2D(Collider2D collision)
    {
        // 画面外に出たら自身を破棄する
        if (collision.gameObject.tag == "ActiveArea")
        {
            Destroy(gameObject);
        }
    }
}