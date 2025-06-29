// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// HomingCluster.cs
// プレイヤーを追尾するクラスターミサイルの挙動を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HomingCluster : MonoBehaviour
{
    public float moveSpeed; // 移動速度
    public float power; // 攻撃力
    GameObject[] Enemy; // シーン内の敵オブジェクト配列
    GameObject Boss; // シーン内のボスオブジェクト
    GameObject player; // プレイヤーオブジェクト
    public GameObject explosion; // 爆発エフェクトのプレハブ
    public GameObject NearestEnemy; // 最も近い敵
    Rigidbody2D rigidBody; // Rigidbody2Dコンポーネント
    public float angleChangingSpeed; // 追尾時の旋回速度
    
    // ミサイルの状態
    public enum Mode
    {
        initial, // 初期拡散フェーズ
        chasing, // 追尾フェーズ
    };

    public Mode mode; // 現在の状態
    float timer; // 経過時間タイマー

    public Vector3 startPos; // 初期拡散時の目標位置
    Vector2 tempPos; // プレイヤーを見失った時の目標位置
   
    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player");
        mode = Mode.initial; // 初期モード設定
        // 発射地点の周囲にランダムな目標地点を設定
        Vector2 randPos = Random.insideUnitCircle * 2;
        startPos = new Vector3(randPos.x + transform.position.x, (-Mathf.Abs(randPos.y)) + transform.position.y, 0);
        timer = 0;
        rigidBody = GetComponent<Rigidbody2D>();
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        player = GameObject.FindGameObjectWithTag("Player");
        if (player != null)
        {
            tempPos = player.transform.position; // プレイヤーの位置を保存
        }

        timer += Time.deltaTime; // タイマー更新

        // === 最も近い敵を探索 ===
        Vector3 pos = transform.position;
        float dist = float.PositiveInfinity;
        Enemy = GameObject.FindGameObjectsWithTag("Enemy");
        Boss = GameObject.FindGameObjectWithTag("Boss");
        // 通常の敵から最も近いものを探す
        foreach (var obj in Enemy)
        {
            var d = (pos - obj.transform.position).sqrMagnitude;
            if (d < dist)
            {
                NearestEnemy = obj;
                dist = d;
            }
        }
        // ボスがいて、かつ通常の敵より近い場合はボスをターゲットにする
        if (Boss != null)
        {
            var bossDis = (pos - Boss.transform.position).sqrMagnitude;
            if (bossDis < dist)
            {
                NearestEnemy = Boss;
            }
        }

        // === 移動処理 ===
        // ターゲット（最も近い敵）がいる場合
        if (NearestEnemy != null)
        {
            // 初期拡散フェーズ
            if (mode == Mode.initial)
            {
                // ランダムな目標地点に向かって直進
                Vector2 dir = (Vector2)startPos - (Vector2)transform.position;
                transform.Translate(dir.normalized * 2 * Time.deltaTime, Space.World); // ワールド座標系で移動
            }
            // 追尾フェーズ
            else
            {
                // ターゲットに向かって旋回しながら追尾
                Vector2 direction = (Vector2)NearestEnemy.transform.position - rigidBody.position;
                direction.Normalize();
                float rotateAmount = Vector3.Cross(direction, transform.up).z;
                rigidBody.angularVelocity = -angleChangingSpeed * rotateAmount;
                rigidBody.velocity = transform.up * moveSpeed;
            }
        }
        // ターゲットがいない場合
        else
        {
            // 最後に確認したプレイヤーの位置に向かって追尾
            Vector2 direction = tempPos - rigidBody.position;
            direction.Normalize();
            float rotateAmount = Vector3.Cross(direction, transform.up).z;
            rigidBody.angularVelocity = -angleChangingSpeed * rotateAmount;
            rigidBody.velocity = transform.up * moveSpeed;
        }

        // 1秒後に追尾フェーズに移行
        if (timer >= 1)
        {
            mode = Mode.chasing;
        }
        // 10秒後に自爆
        if (timer >= 10)
        {
            if (explosion != null)
            {
                Instantiate(explosion, transform.position, Quaternion.identity);
            }
            Destroy(gameObject);
        }
    }

    // 他のコライダーと接触した瞬間に呼ばれます
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Enemy" || collision.gameObject.tag == "Boss")
        {
            // 相手にダメージを与えて自身は消滅
            HP hit = collision.gameObject.GetComponent<HP>();
            if (hit != null)
            {
                hit.Damage(power);
            }
            Destroy(gameObject);
        }
    }

    // 画面外に出たら自身を破棄
    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "ActiveArea")
        {
            Destroy(gameObject);
        }
    }
}