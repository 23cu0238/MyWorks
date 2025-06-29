// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Stage3.cs
// ステージ3の敵の出現パターンや進行を管理するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Stage3 : MonoBehaviour
{
    // UIとゲームオブジェクトへの参照
    public Text TimeText; // 現在は未使用
    public float timeLimit; // 現在は未使用
    public GameObject enemyPrefab; // 敵タイプ1
    public GameObject enemy2Prefab; // 敵タイプ2
    public GameObject enemy3Prefab; // 敵タイプ3
    public GameObject BossPrefab; // ボス
    public GameObject CautionPrefab; // 警告サイン
    public int enemyNumber; // (現在は未使用)
    
    // ステージ進行管理用の変数
    ProcessManager pm;
    bool BossSpawned;
    public GameObject boss;

    // ステージの状態
    public enum Status
    {
        normal,
        boss,
    };
    Status status;

    // 敵の出現位置
    GameObject[] SP;

    // 敵の出現ウェーブ管理
    public int wave;
    float spawnTimer;

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // コンポーネントを取得
        pm = GetComponent<ProcessManager>();
        
        // 初期状態を設定
        status = Status.normal;
        wave = 1;
        spawnTimer = 0;
        BossSpawned = false;

        // 敵の出現位置(Spawn Point)を名前で検索して配列に格納
        {
            SP = new GameObject[19];
            for (int i = 0; i <= 9; i++) { SP[i] = GameObject.Find("SP" + i); }
            for (int i = 1; i <= 9; i++) { SP[9 + i] = GameObject.Find("SP-" + i); }
        }
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // 進行度がターゲットに達したらボス戦に移行
        if (pm.process >= pm.target)
        {
            status = Status.boss;
        }

        // 通常状態の処理
        if (status == Status.normal)
        {
            spawnTimer -= Time.deltaTime;

            // タイマーが0以下になったら次のウェーブを開始
            if (spawnTimer <= 0)
            {
                // ウェーブ数に応じて異なる敵の組み合わせをスポーン
                switch (wave)
                {
                    case 1:
                        StartCoroutine(SpActivate(SP[0], enemy2Prefab, 10));
                        StartCoroutine(SpActivate(SP[2], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[11], enemyPrefab, 10));
                        spawnTimer = 5;
                        wave++;
                        break;

                    case 2:
                        StartCoroutine(SpActivate(SP[1], enemy2Prefab, 10));
                        StartCoroutine(SpActivate(SP[10], enemy3Prefab, 10));
                        StartCoroutine(SpActivate(SP[5], enemy2Prefab, 10));
                        StartCoroutine(SpActivate(SP[14], enemy3Prefab, 10));
                        spawnTimer = 5;
                        wave++;
                        break;

                    case 3:
                        StartCoroutine(SpActivate(SP[8], enemy2Prefab, 15));
                        StartCoroutine(SpActivate(SP[17], enemy2Prefab, 15));
                        spawnTimer = 5;
                        wave++;
                        break;

                    case 4:
                        StartCoroutine(SpActivate(SP[3], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[4], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[6], enemy3Prefab, 6));
                        StartCoroutine(SpActivate(SP[12], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[13], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[15], enemy3Prefab, 6));
                        spawnTimer = 5;
                        wave++;
                        break;

                    case 5:
                        StartCoroutine(SpActivate(SP[7], enemy2Prefab, 10));
                        StartCoroutine(SpActivate(SP[9], enemy3Prefab, 10));
                        StartCoroutine(SpActivate(SP[16], enemy2Prefab, 10));
                        StartCoroutine(SpActivate(SP[18], enemy3Prefab, 10));
                        spawnTimer = 5;
                        wave = 1; // 最後のウェーブが終わったら最初に戻る
                        break;
                        
                    default:
                        wave = 1;
                        break;
                }
            }
        }
        // ボス戦状態の処理
        else if (status == Status.boss)
        {
            // 残っている雑魚敵を全て破壊
            GameObject[] EnemyLeft = GameObject.FindGameObjectsWithTag("Enemy");
            foreach (var obj in EnemyLeft)
            {
                Destroy(obj.gameObject);
            }
            
            // ボスがまだ出現していなければ、出現処理を開始
            if (BossSpawned == false)
            {
                StartCoroutine(SummonBoss());
                BossSpawned = true;
            }
        }
    }

    // ボスを召喚するコルーチン
    private IEnumerator SummonBoss()
    {
        Instantiate(CautionPrefab, new Vector3(0, 0, 0), Quaternion.identity);
        yield return new WaitForSeconds(2f);
        Instantiate(BossPrefab, SP[0].transform.position, Quaternion.identity);
        yield return new WaitForSeconds(2f);
        BossSpawned = true;
    }

    // 指定されたスポーンポイントから敵を定期的に出現させる汎用コルーチン
    private IEnumerator SpActivate(GameObject spawnPoint, GameObject enemy, float interval)
    {
        do
        {
            if (spawnPoint)
            {
                Instantiate(enemy, spawnPoint.transform.position, Quaternion.identity);
            }
            yield return new WaitForSeconds(interval);
        } while (status == Status.normal);
    }
}