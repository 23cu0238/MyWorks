// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Stage1.cs
// ステージ1の敵の出現パターンや進行を管理するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Stage1 : MonoBehaviour
{
    // UIとゲームオブジェクトへの参照
    public Text TimeText; // 現在は未使用ですが、時間表示用のUIテキスト
    public float timeLimit; // 現在は未使用ですが、制限時間用の変数
    public GameObject enemyPrefab; // 出現させる敵のプレハブ
    public GameObject BossPrefab; // 出現させるボスのプレハブ
    public GameObject CautionPrefab; // ボス出現前に表示する警告サインのプレハブ
    
    // ステージ進行管理用の変数
    ShootingGameManager mng; // ゲームマネージャー
    ProcessManager pm; // 進行度マネージャー
    bool BossSpawned; // ボスが既に出現したかどうかのフラグ
    public GameObject boss; // 出現したボスオブジェクトへの参照

    // ステージの状態を管理するenum
    public enum Status
    {
        normal, // 通常の敵が出現する状態
        boss,   // ボス戦の状態
    };
    Status status;

    // 敵の出現位置を保持する配列
    GameObject[] SP;

    // 敵の出現ウェーブ管理
    public int wave; // 現在のウェーブ数
    float spawnTimer; // 次のウェーブまでのタイマー
    float endTimer; // (現在は未使用)

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // コンポーネントを取得
        pm = GetComponent<ProcessManager>();
        mng = GameObject.Find("Main Camera").GetComponent<ShootingGameManager>();
        
        // 初期状態を設定
        status = Status.normal;
        wave = 1;
        spawnTimer = 0;
        BossSpawned = false;

        // 敵の出現位置(Spawn Point)を名前で検索して配列に格納
        {
            SP = new GameObject[19];
            for (int i = 0; i <= 9; i++)
            {
                SP[i] = GameObject.Find("SP" + i);
            }
            for (int i = 1; i <= 9; i++)
            {
                SP[9 + i] = GameObject.Find("SP-" + i);
            }
        }
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // 進行度がターゲットに達したらボス戦に移行
        if (pm.process>=pm.target)
        {
            status = Status.boss;
        }

        // 通常状態の処理
        if (status == Status.normal)
        {
            spawnTimer -= Time.deltaTime; // スポーンタイマーを減らす

            // タイマーが0以下になったら次のウェーブを開始
            if (spawnTimer <= 0)
            {
                // 現在のウェーブ数に応じて敵の出現パターンを決定
                switch (wave)
                {
                    case 1:
                        StartCoroutine(SpActivate(SP[0], enemyPrefab, 10)); // 名称を汎用化
                        StartCoroutine(SpActivate(SP[2], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[11], enemyPrefab, 10));
                        spawnTimer = 5;
                        wave++;
                        break;
                    case 2:
                        StartCoroutine(SpActivate(SP[1], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[10], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[5], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[14], enemyPrefab, 10));
                        spawnTimer = 5;
                        wave++;
                        break;
                    // ...他のウェーブも同様...
                    case 3:
                        StartCoroutine(SpActivate(SP[8], enemyPrefab, 15));
                        StartCoroutine(SpActivate(SP[17], enemyPrefab, 15));
                        spawnTimer = 5;
                        wave++;
                        break;
                    case 4:
                        StartCoroutine(SpActivate(SP[3], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[4], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[6], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[12], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[13], enemyPrefab, 6));
                        StartCoroutine(SpActivate(SP[15], enemyPrefab, 6));
                        spawnTimer = 5;
                        wave++;
                        break;
                    case 5:
                        StartCoroutine(SpActivate(SP[7], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[9], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[16], enemyPrefab, 10));
                        StartCoroutine(SpActivate(SP[18], enemyPrefab, 10));
                        spawnTimer = 5;
                        wave++; // これ以上ウェーブがない場合、ループを続けるか検討
                        break;
                    default:
                        // 全てのウェーブが終了したら、タイマーをリセットしてループ
                        wave = 1;
                        spawnTimer = 10;
                        break;
                }
            }
        }
        // ボス戦状態の処理
        else if(status==Status.boss)
        {
            // 残っている雑魚敵を全て破壊
            GameObject[] EnemyLeft = GameObject.FindGameObjectsWithTag("Enemy");
            foreach(var obj in EnemyLeft)
            {
                Destroy(obj.gameObject);
            }
            
            // ボスがまだ出現していなければ、出現処理を開始
            if (BossSpawned==false)
            {
                StartCoroutine(SummonBoss());
                BossSpawned = true;
            }
        }
    }

    // ボスを召喚するコルーチン
    private IEnumerator SummonBoss()
    {
        // 警告サインを表示
        Instantiate(CautionPrefab, new Vector3(0,0,0), Quaternion.identity);
        // 2秒待機
        yield return new WaitForSeconds(2f);
        // ボスを出現させる
        Instantiate(BossPrefab, SP[0].transform.position, Quaternion.identity);
        yield return new WaitForSeconds(2f); // ボス出現後の待機（任意）
        BossSpawned = true;
    }

    // 指定されたスポーンポイントから敵を定期的に出現させる汎用コルーチン
    private IEnumerator SpActivate(GameObject spawnPoint, GameObject enemy, float interval)
    {
        // 通常状態でいる限り、処理を繰り返す
        do
        {
            Instantiate(enemy, spawnPoint.transform.position, Quaternion.identity);
            yield return new WaitForSeconds(interval);
        } while (status == Status.normal);
    }
}