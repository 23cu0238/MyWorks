// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Boss.cs
// エキストラステージのボスの行動パターンを制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Boss : MonoBehaviour
{
    public float EndTimer; // 撃破後の終了シーケンス用タイマー
    public float moveSpeed; // 移動速度
    public float boundaries; // 左右の移動限界
    public GameObject bulletPrefab; // 発射する弾のプレハブ
    public GameObject Ending; // エンディング演出用のプレハブ
    public float bulletInterval; // 弾の発射間隔
    Vector3 bulletTransform; // 弾の発射位置
    Animator animator; // アニメーターコンポーネント

    // ボスの位置状態
    public enum Bpos
    {
        center,
        left,
        right
    };
    // ボスの移動方向
    public enum Bdirection
    {
        towardsLeft,
        towardsRight
    };
    // ボスの行動状態
    public enum Bstatus
    {
        moving,
        stop
    }

    public Bpos BossPos; // 現在の位置状態
    public Bdirection BossDirection; // 現在の移動方向
    public Bstatus BossStatus; // 現在の行動状態
    bool isShot; // 攻撃アニメーション中に一回だけ弾を発射するためのフラグ
    public bool IsEnd; // 撃破されたかどうかのフラグ
    SpriteRenderer spr; // スプライト描画用コンポーネント
    Vector3 centerPos; // 中央位置の保存用

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        animator = GetComponent<Animator>();
        // 各状態の初期化
        BossPos = Bpos.center;
        BossDirection = Bdirection.towardsLeft;
        BossStatus = Bstatus.moving;
        isShot = false;
        IsEnd = false;
        spr = GetComponent<SpriteRenderer>();
        // 中央位置を保存
        centerPos = new Vector3(0.0f,transform.position.y,transform.position.z);
        EndTimer = 0;
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // 撃破されていない場合
        if (IsEnd == false)
        {
            // === 中央にいるときの処理 ===
            if (BossPos == Bpos.center)
            {
                // 移動中の場合
                if (BossStatus == Bstatus.moving)
                {
                    // 左へ移動
                    if (BossDirection == Bdirection.towardsLeft)
                    {
                        if (transform.position.x > -boundaries)
                        {
                            transform.Translate(-moveSpeed, 0.0f, 0.0f);
                            isShot = false;
                        }
                        // 左端に到達したら停止し、攻撃アニメーション再生
                        if (transform.position.x <= -boundaries)
                        {
                            animator.Play("orga_shoot");
                            BossPos = Bpos.left;
                            BossStatus = Bstatus.stop;
                        }
                    }
                    // 右へ移動
                    if (BossDirection == Bdirection.towardsRight)
                    {
                        if (transform.position.x < boundaries)
                        {
                            transform.Translate(moveSpeed, 0.0f, 0.0f);
                            isShot = false;
                        }
                        // 右端に到達したら停止し、攻撃アニメーション再生
                        if (transform.position.x >= boundaries)
                        {
                            animator.Play("orga_shoot");
                            BossPos = Bpos.right;
                            BossStatus = Bstatus.stop;
                        }
                    }
                }
                // 停止中で、かつ攻撃済みの場合
                else if (BossStatus == Bstatus.stop && isShot == true)
                {
                    // アイドルアニメーションに戻ったら、再度移動を開始
                    if (spr.sprite.name == "ezgif.com-gif-maker_1") // 特定のスプライト名に依存するのは不安定な可能性あり
                    {
                        BossStatus = Bstatus.moving;
                    }
                }
            }

            // === 左端にいるときの処理 ===
            if (BossPos == Bpos.left)
            {
                // 停止中で、かつ攻撃済みの場合
                if (BossStatus == Bstatus.stop)
                {
                     // アイドルアニメーションに戻ったら、右へ移動を開始
                    if (spr.sprite.name == "ezgif.com-gif-maker_0" && isShot == true)
                    {
                        BossDirection = Bdirection.towardsRight;
                        BossStatus = Bstatus.moving;
                    }
                }
                // 移動中の場合
                if (BossStatus == Bstatus.moving)
                {
                    // 中央に向かって移動
                    if (transform.position.x < boundaries)
                    {
                        transform.Translate(moveSpeed, 0.0f, 0.0f);
                        isShot = false;
                    }
                    // 中央に到達したら停止し、攻撃アニメーション再生
                    if (transform.position.x >= 0.0f)
                    {
                        animator.Play("orga_shoot");
                        transform.position = centerPos;
                        BossPos = Bpos.center;
                        BossStatus = Bstatus.stop;
                    }
                }
            }
            
            // === 右端にいるときの処理 ===
            if (BossPos == Bpos.right)
            {
                // 停止中で、かつ攻撃済みの場合
                if (BossStatus == Bstatus.stop)
                {
                    // アイドルアニメーションに戻ったら、左へ移動を開始
                    if (spr.sprite.name == "ezgif.com-gif-maker_0" && isShot == true)
                    {
                        BossDirection = Bdirection.towardsLeft;
                        BossStatus = Bstatus.moving;
                    }
                }
                 // 移動中の場合
                if (BossStatus == Bstatus.moving)
                {
                    // 中央に向かって移動
                    if (transform.position.x > -boundaries)
                    {
                        transform.Translate(-moveSpeed, 0.0f, 0.0f);
                        isShot = false;
                    }
                     // 中央に到達したら停止し、攻撃アニメーション再生
                    if (transform.position.x <= 0.0f)
                    {
                        animator.Play("orga_shoot");
                        transform.position = centerPos;
                        BossPos = Bpos.center;
                        BossStatus = Bstatus.stop;
                    }
                }
            }

            // 攻撃アニメーションの特定フレームで弾を発射
            if (spr.sprite.name == "orga_shoot_25" && isShot == false)
            {
                Shoot();
            }
        }
        // 撃破された後の処理
        else if (IsEnd == true)
        {
            EndTimer += Time.deltaTime;
            // 5秒後にエンディング演出を生成
            if (EndTimer >= 5)
            {
                Instantiate(Ending, new Vector3(0, 0, 0), Quaternion.identity);
                EndTimer = 0; // 複数回生成しないようにリセット
            }    
        }
    }

    // 弾を発射する関数
    public void Shoot()
    {
        bulletTransform = new Vector3(transform.position.x, transform.position.y, transform.position.z);
        // 3-way弾を発射
        Instantiate(bulletPrefab, bulletTransform, Quaternion.identity);
        Instantiate(bulletPrefab, bulletTransform, Quaternion.AngleAxis(30, new Vector3(0.0f, 0.0f, 1.0f)));
        Instantiate(bulletPrefab, bulletTransform, Quaternion.AngleAxis(-30, new Vector3(0.0f, 0.0f, 1.0f)));

        isShot = true; // 発射済みフラグを立てる
    }
    
    // 撃破シーケンスを開始する関数
    public void EndEx()
    {
        IsEnd = true; // 撃破フラグを立てる
        animator.Play("orga_down"); // ダウンアニメーションを再生
    }
}