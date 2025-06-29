// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// LaserBeam.cs
// プレイヤーのレーザー武器の挙動を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LaserBeam : MonoBehaviour
{
    public int power; // レーザーの秒間ダメージ
    private GameObject player; // プレイヤーオブジェクトへの参照
    float hitInterval = 0.1f; // ダメージを与える間隔
    float hitCountdown; // 次のダメージまでのカウントダウンタイマー

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player");
        hitCountdown = 0;
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // プレイヤーが存在する場合
        if(player != null)
        {
            // プレイヤーの位置に合わせて自身の位置を更新する
            Vector3 newPos = new Vector3(player.transform.position.x, player.transform.position.y + 7.0f, 0.0f);
            transform.position = newPos;
        }
        else
        {
            // プレイヤーがいない場合は自身も破棄
            Destroy(gameObject);
            return;
        }

        // ダメージ間隔タイマーを更新
        hitCountdown += Time.deltaTime;
    }

    // 他のコライダーと接触している間、毎フレーム呼ばれます
    private void OnTriggerStay2D(Collider2D collision)
    {
        // ダメージ間隔が経過している場合
        if (hitCountdown >= hitInterval)
        {
            // 接触相手が敵またはボスの場合
            if (collision.gameObject.tag == "Enemy" || collision.gameObject.tag == "Boss")
            {
                // 相手のHPコンポーネントを取得し、ダメージを与える
                HP hit = collision.gameObject.GetComponent<HP>();
                if (hit != null)
                {
                    hit.Damage(power);
                }
                // ダメージを与えたらタイマーをリセット
                hitCountdown = 0;
            }
        }
    }
}