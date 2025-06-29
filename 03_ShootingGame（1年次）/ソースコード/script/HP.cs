// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// HP.cs
// 敵やオブジェクトの体力（HP）を管理する汎用コンポーネント

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HP : MonoBehaviour
{
    public float enemyHp; // HPの現在値
    private bool DropIsEnabled; // アイテムドロップが有効かどうかのフラグ
    public GameObject explosionPrefab; // 通常の爆発エフェクトのプレハブ
    public GameObject itemPrefab; // パワーアップアイテムのプレハブ
    public GameObject LifePrefab; // ライフアップアイテムのプレハブ
    public GameObject EMPPrefab; // EMPアイテムのプレハブ
    public GameObject EMPExplosion; // ボス撃破時のEMP爆発エフェクト
    public int scoreEarned; // この敵を倒したときに得られるスコア
    public float itemDropRate; // パワーアップアイテムのドロップ率
    public float lifeDropRate; // ライフアップアイテムのドロップ率
    public float EMPDropRate; // EMPアイテムのドロップ率
    float DamageTimer; // ダメージを受けた後の色変更タイマー
    bool IsDamaged; // ダメージを受けている最中かどうかのフラグ
    bool IsEnd; // ボスが撃破シーケンス中かどうかのフラグ
    private int n; // ドロップ判定用の乱数
    SpriteRenderer sr; // スプライト描画用コンポーネント
    ProcessManager pm; // ステージ進行度マネージャー
    public int ProcessValue; // この敵を倒したときの進行度上昇値

    public float LifeDropRate { get => lifeDropRate; set => lifeDropRate = value; } // プロパティ（現在未使用）

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        sr = GetComponent<SpriteRenderer>();
        DropIsEnabled = true;
        DamageTimer = 0;
        IsDamaged = false;
        IsEnd = false;
        pm = GameObject.Find("Main Camera")?.GetComponent<ProcessManager>(); // ?.でnullチェックを簡略化
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // === HPが0以下になった時の処理 ===
        // ボス("Orga")が撃破され、まだ終了シーケンスに入っていない場合
        if (enemyHp <= 0 && gameObject.name == "Orga" && IsEnd == false)
        {
            enemyHp = 0; // HPがマイナスにならないように
            BossEnd(); // ボス撃破シーケンスを開始
            IsEnd = true;
        }
        // 通常の敵("Enemy"タグ)が撃破された場合
        else if(enemyHp <= 0 && gameObject.tag == "Enemy")
        {
            explosion(); // 通常の爆発処理
        }
        
        // === ダメージを受けた時の色変更処理 ===
        if (IsDamaged == true)
        {
            // スプライトを赤みがかった色にする
            sr.color = new Color(1f, 0.46f, 0.52f, 1f);
            DamageTimer += Time.deltaTime;

            // 0.08秒後に元の色に戻す
            if (DamageTimer > 0.08)
            {
                sr.color = Color.white;
                IsDamaged = false;
                DamageTimer = 0;
            }
        }
    }

    // ダメージを受ける関数
    public void Damage(float damage)
    {
        enemyHp -= damage;
    }

    // 通常の敵が破壊されたときの処理
    public void explosion()
    {
        // 進行度をプラス
        if (pm != null)
        {
            pm.ProcessPlus(ProcessValue);
        }
        
        // 爆発エフェクトを生成
        Instantiate(explosionPrefab, transform.position, Quaternion.identity);

        // スコアを加算
        ShootingGameManager mng = GameObject.Find("Main Camera")?.GetComponent<ShootingGameManager>();
        if (mng != null)
        {
            mng.AddScore(scoreEarned);
        }
        
        // === アイテムドロップ判定 ===
        if (DropIsEnabled == true)
        {
            n = Random.Range(1, 101); // 1から100までの乱数
            // 各アイテムのドロップ率に基づいて判定し、生成
            if (n <= itemDropRate)
            {
                Instantiate(itemPrefab, transform.position, Quaternion.identity);
            }
            if (n <= LifeDropRate)
            {
                Instantiate(LifePrefab, transform.position, Quaternion.identity);
            }
            if (n <= EMPDropRate)
            {
                Instantiate(EMPPrefab, transform.position, Quaternion.identity);
            }
        }

        // 最後に自身を破棄
        Destroy(gameObject);
    }

    // 他のコライダーと接触した瞬間に呼ばれます
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // EMP爆風に当たった場合、アイテムをドロップしないようにする
        if (collision.gameObject.tag == "EMPExplosion")
        {
            DropIsEnabled = false;
        }
        // プレイヤーの弾に当たった場合、ダメージ色変更フラグを立てる
        if (collision.gameObject.tag == "PlayerBullet" || collision.gameObject.tag == "PlayerSubBullet")
        {
            IsDamaged = true;
            DamageTimer = 0; // タイマーリセット
        }
    }

    // 他のコライダーと接触している間、毎フレーム呼ばれます
    private void OnTriggerStay2D(Collider2D collision)
    {
        // プレイヤーのレーザーに当たっている間、ダメージ色変更フラグを立て続ける
        if (collision.gameObject.name == "Laser(clone)") // 名前での比較は不安定なため、タグやレイヤーの使用を推奨
        {
            IsDamaged = true;
            DamageTimer = 0;
        }
    }

    // ボスが撃破されたときの専用処理
    public void BossEnd()
    {
        Instantiate(EMPExplosion, transform.position, Quaternion.identity);
        Stage_Ex ex =  GameObject.Find("Main Camera")?.GetComponent<Stage_Ex>();
        if(ex != null) ex.EndEx();
        
        Boss bs = GetComponent<Boss>();
        if(bs != null) bs.EndEx();
        
        Player pl = GameObject.Find("Player")?.GetComponent<Player>();
        if(pl != null) pl.InvincibleTimer = 999; // プレイヤーを無敵にする
    }
}