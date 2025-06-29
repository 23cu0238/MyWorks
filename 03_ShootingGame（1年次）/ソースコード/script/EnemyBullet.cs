// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// EnemyBullet.cs
// 敵が発射する弾の挙動を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyBullet : MonoBehaviour
{
    public float moveSpeed; // 弾の移動速度

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // Z軸回転に基づいて、自身の前方（ローカル座標のY軸負方向）へ移動
        transform.Translate(0.0f, -moveSpeed * Time.deltaTime, 0.0f);
    }

    // 画面外（ActiveAreaのコライダーから出た）に出たら自身を破棄
    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "ActiveArea")
        {
            Destroy(gameObject);
        }
    }
}