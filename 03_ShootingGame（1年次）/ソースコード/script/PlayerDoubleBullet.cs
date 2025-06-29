// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// PlayerDoubleBullet.cs
// プレイヤーのダブルバレット（未使用か開発中）の挙動を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerDoubleBullet : MonoBehaviour
{
    public float moveSpeed; // 弾の移動速度
    public GameObject explosionPrefab; // 爆発エフェクトのプレハブ
    // public int bulletPower; // 攻撃力（コメントアウトされている）

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {

    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // 自身のY軸正方向（上方向）へ移動
        transform.Translate(0.0f, moveSpeed * Time.deltaTime, 0.0f);
    }

    // 他のコライダーと接触した瞬間に呼ばれます
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // 敵にヒットした場合
        if (collision.gameObject.tag == "Enemy")
        {
            // 自身を破棄
            Destroy(gameObject);
            // ここで敵にダメージを与える処理が必要
        }
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