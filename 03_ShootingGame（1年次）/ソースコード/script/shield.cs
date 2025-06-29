// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// shield.cs
// プレイヤーを一定時間守るシールドの挙動を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class shield : MonoBehaviour
{
    // シールドの持続時間タイマー
    float Timer;
    
    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // タイマーを2秒に初期化
        Timer = 2;
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // 時間経過でタイマーを減少
        Timer -= Time.deltaTime;
        // タイマーが0以下になったらシールドを破壊
        if (Timer<=0)
        {
            Destroy(gameObject);
        }
    }

    // 他のコライダーと接触した時に呼ばれます
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // 接触した相手が敵の場合
        if (collision.gameObject.tag == "Enemy")
        {
            // 敵に大ダメージを与えて破壊する
            HP hit = collision.gameObject.GetComponent<HP>();
            hit.Damage(999);
        }
        // 接触した相手が敵の弾の場合
        if (collision.gameObject.tag == "EnemyBullet")
        {
            // 敵の弾を破壊する
            Destroy(collision.gameObject);
        }
    }
}