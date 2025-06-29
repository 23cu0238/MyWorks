// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// EMPExplosion.cs
// EMPアイテム取得時に発生する爆風の挙動を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EMPExplosion : MonoBehaviour
{
    private float timer; // 経過時間タイマー
    public float ExpandSpeed; // 爆風が拡大する速度
    public float DestroyTime; // 爆風が消えるまでの時間
    public float power; // 爆風が敵に与えるダメージ

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        timer = 0;
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        timer += Time.deltaTime; // 経過時間を加算
        
        // 自身のスケールを拡大させて、爆風が広がるように見せる
        transform.localScale += new Vector3(ExpandSpeed, ExpandSpeed, ExpandSpeed) * Time.deltaTime; // 速度をフレームレートに依存しないように修正

        // 指定時間が経過したら自身を破棄
        if(timer >= DestroyTime)
        {
            Destroy(gameObject);
        }
    }

    // 他のコライダーと接触した瞬間に呼ばれます
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // 画面内の敵、またはボスにヒットした場合
        if ((collision.gameObject.tag == "Enemy" && collision.gameObject.transform.position.y <= 4.5) || collision.gameObject.tag == "Boss")
        {
            // 相手のHPコンポーネントを取得し、ダメージを与える
            HP hit = collision.gameObject.GetComponent<HP>();
            if (hit != null)
            {
                hit.Damage(power);
            }
        }
        // 敵の弾にヒットした場合
        if(collision.gameObject.tag == "EnemyBullet")
        {
            // 弾を消す
            Destroy(collision.gameObject);
        }
    }
}