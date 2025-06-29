// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// PlayerBullet.cs
// プレイヤーが発射する通常弾の挙動を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerBullet : MonoBehaviour
{
    public float moveSpeed; // 弾の移動速度
    public float power; // 弾の攻撃力

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
        // 敵またはボスにヒットした場合
        if (collision.gameObject.tag == "Enemy" || collision.gameObject.tag == "Boss")
        {
            // 自身を破棄
            Destroy(gameObject);

            // 相手のHPコンポーネントを取得し、ダメージを与える
            HP hit = collision.gameObject.GetComponent<HP>();
            if (hit != null)
            {
                hit.Damage(power);
            }
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