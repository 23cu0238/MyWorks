// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Stage_Ex.cs
// エキストラステージの進行を管理するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Stage_Ex : MonoBehaviour
{
    // UIとゲームオブジェクトへの参照
    public Text TimeText; // 経過時間を表示するUIテキスト
    float gameTimer; // 経過時間タイマー
    public GameObject enemyPrefab; // 出現させる敵のプレハブ
    public int enemyNumber; // 画面内に維持する敵の数
    float enemyRadius; // 敵の半径（スポーン時の衝突判定用）

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        gameTimer = 0; // タイマー初期化

        // ゲームマネージャーからスコアを読み込んで表示を更新
        ShootingGameManager mng = GameObject.Find("Main Camera").GetComponent<ShootingGameManager>();
        mng.renewScore();

        // 敵のコライダーの半径を取得
        enemyRadius = enemyPrefab.GetComponent<Collider2D>().bounds.extents.x;

        // 画面内の敵が指定数に満たない場合、補充する
        for (int i = GameObject.FindGameObjectsWithTag("Enemy").Length; i < enemyNumber; i = GameObject.FindGameObjectsWithTag("Enemy").Length)
        {
            // 画面上部にランダムな位置を生成
            Vector3 spawnPosition = new Vector3(Random.Range(-5.0f, 5.0f), Random.Range(2.5f, 4.5f), 10.0f);

            // スポーン位置に他の敵がいないかチェック
            Collider2D CollisionWithEnemy = Physics2D.OverlapCircle(spawnPosition, enemyRadius, LayerMask.GetMask("EnemyLayer"));

            // 衝突がなければ敵を生成
            if (CollisionWithEnemy == false)
            {
                Instantiate(enemyPrefab, spawnPosition, transform.rotation);
            }
        }
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // 経過時間をUIに表示
        TimeText.text = "Time Used:" + (int)gameTimer;

        // 特定の敵(Enemy04)が存在しない場合 (クリア条件か？)
        if (GameObject.Find("Enemy04") == null)
        {
            // クリアシーンへの遷移処理（現在はコメントアウト）
            // SceneManager.LoadScene("StageEx_Clear");
        }
        // 特定の敵が存在する場合
        else if (GameObject.Find("Enemy04") != null)
        {
            // 経過時間を加算
            gameTimer += Time.deltaTime;

            // 画面内の敵が指定数より少ない場合、補充する
            if (GameObject.FindGameObjectsWithTag("Enemy").Length < enemyNumber)
            {
                Spawn();
            }
        }
    }

    // 敵を一体スポーンさせる関数
    void Spawn()
    {
        // 画面上端にランダムな位置を生成
        Vector3 spawnPosition = new Vector3(Random.Range(-5.0f, 5.0f), 5.0f, 10.0f);

        // スポーン位置に他の敵がいないかチェック
        Collider2D CollisionWithEnemy = Physics2D.OverlapCircle(spawnPosition, enemyRadius, LayerMask.GetMask("EnemyLayer"));

        // 衝突がなければ敵を生成
        if (CollisionWithEnemy == false)
        {
            Instantiate(enemyPrefab, spawnPosition, transform.rotation);
        }
    }

    // EXステージを終了させる外部から呼び出すための関数
    public void EndEx()
    {
        // これ以上敵がスポーンしないように、目標数を0にする
        enemyNumber = 0;
    }
}