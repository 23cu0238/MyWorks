// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Boss1.cs
// ステージ1のボスの行動パターンや弾幕を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Boss1 : MonoBehaviour
{
    public CanvasControl canvas; // UIキャンバス制御スクリプト
    public float NormalMoveSpeed; // 通常の移動速度
    Player player; // プレイヤーオブジェクトへの参照
    public GameObject bulletPrefab; // 発射する弾のプレハブ
    public GameObject explosionPrefab; // 爆発エフェクトのプレハブ
    public GameObject BossHp; // ボスHPバーのUIオブジェクト
    public GameObject item; // ドロップするアイテムのプレハブ
    float bulletTimer; // 弾の発射タイミングを管理するタイマー
    public float bulletInterval; // 弾の発射間隔
    ShootingGameManager mng; // ゲーム全体のマネージャー

    float hp; // 現在のHP
    Vector2 dir; // 移動方向

    // ボスの行動モードを定義する列挙体
    enum Status { vert, hori } // vert: 垂直移動, hori: 水平移動
    Status MovementMode; // 現在の行動モード

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // 定期的にアイテムをドロップするコルーチンを開始
        StartCoroutine(ItemDrop());
        
        // UIキャンバスを取得し、ボスHPバーを有効化
        canvas = GameObject.Find("Canvas").GetComponent<CanvasControl>();
        canvas.ActivateHPBar();

        // 必要なオブジェクトやコンポーネントを取得
        player = GameObject.FindGameObjectWithTag("Player").GetComponent<Player>();
        MovementMode = Status.vert; // 初期モードは垂直移動
        bulletTimer = 0;
        hp = GetComponent<HP>().enemyHp;
        mng = GameObject.Find("Main Camera").GetComponent<ShootingGameManager>();
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        player = GameObject.FindGameObjectWithTag("Player")?.GetComponent<Player>(); // プレイヤーがいない場合も考慮

        // Y座標が2.4以下になったら水平移動モードに移行
        if (transform.position.y <= 2.4)
        {
            MovementMode = Status.hori;
        }

        // 垂直移動モードの場合
        if (MovementMode == Status.vert)
        {
            // 真下に移動
            transform.Translate(0, -NormalMoveSpeed, 0);
        }
        
        // 水平移動モードの場合
        if (MovementMode == Status.hori)
        {
            // プレイヤーが存在すれば、プレイヤーのX座標を追うように移動
            if (player != null)
            { 
                dir = new Vector2(player.transform.position.x - transform.position.x, 0); // Yは追わない
            }
            transform.Translate(dir.normalized.x * NormalMoveSpeed, 0f, 0f);
        }

        // 弾発射タイマーを更新
        bulletTimer += Time.deltaTime;

        // 発射間隔に達したら弾を発射
        if (bulletTimer >= bulletInterval)
        {
            // ランダムな角度（前方下向き）に弾を3つ発射
            for (int i = 0; i < 3; ++i)
            {
                Instantiate(bulletPrefab, transform.position, Quaternion.Euler(0, 0, Random.Range(135f, 225f)));
            }
            bulletTimer = 0.0f; // タイマーリセット
        }

        // HPを更新
        hp = GetComponent<HP>().enemyHp;
        
        // HPが0以下になったら
        if (hp <= 0)
        {
            // 爆発エフェクトを生成
            Instantiate(explosionPrefab, transform.position, Quaternion.identity);
            // 現在のスコアやライフを保存
            mng.SaveInfo();
            // ステージクリア処理を開始
            mng.LoadClear();
            // ボス自身を破棄
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

    // 定期的にアイテムをドロップするコルーチン
    private IEnumerator ItemDrop()
    {
        // 無限ループ
        do
        {
            // 0から99の乱数を生成
            float rand = Random.Range(0, 99);
            // 5%の確率でアイテムを生成
            if (rand <= 5)
            {
                Instantiate(item, transform.position, Quaternion.identity);
            }
            // 1秒待機
            yield return new WaitForSeconds(1f);
        }
        while (true);
    }
}