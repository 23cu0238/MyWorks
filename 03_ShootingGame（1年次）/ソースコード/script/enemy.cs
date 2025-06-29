// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// enemy.cs
// 通常の敵キャラクターの基本的な動きと攻撃を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class enemy : MonoBehaviour
{
    public float NormalMoveSpeed; // 基本の移動速度
    float HoriMoveSpeed; // 水平方向の移動速度
    public GameObject bulletPrefab; // 発射する弾のプレハブ
    float bulletTimer; // 弾の発射タイミングを管理するタイマー
    public float bulletInterval; // 弾の発射間隔
    
    // 敵の行動モードを定義する列挙体
    enum Status { vert, hori } // vert: 垂直移動, hori: 水平移動
    Status MovementMode; // 現在の行動モード

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        HoriMoveSpeed = NormalMoveSpeed;

        // 出現位置によって初期の水平移動方向を決定
        if (transform.position.x > -6.5 && transform.position.x < 6.5)
        {
            HoriMoveSpeed = 0; // 中央付近なら水平移動なし
        }
        else if (transform.position.x > 6.5)
        {
            HoriMoveSpeed = -HoriMoveSpeed; // 右端なら左へ
        }
        // 左端ならそのままの速度(右へ)
       
        MovementMode = enemy.Status.vert; // 初期モードは垂直移動
        bulletTimer = 0;
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // Y座標が-1以下になったら水平移動モードに移行
        if(transform.position.y <= -1)
        {
            MovementMode = Status.hori;
        }

        // 垂直移動モードの場合
        if (MovementMode == Status.vert)
        {
            // 設定された速度で斜め下または真下に移動
            transform.Translate(HoriMoveSpeed * Time.deltaTime, -NormalMoveSpeed * Time.deltaTime, 0.0f);
        }
        
        // 水平移動モードの場合
        if (MovementMode == Status.hori)
        {
            // 画面中央から左右に分かれて画面外へ移動
            if (transform.position.x >= 0)
            {
                transform.Translate(NormalMoveSpeed * Time.deltaTime, 0.0f, 0.0f); // 右へ
            }
            if (transform.position.x < 0)
            {
                transform.Translate(-NormalMoveSpeed * Time.deltaTime, 0.0f, 0.0f); // 左へ
            }
        }

        // 弾発射タイマーを更新
        bulletTimer += Time.deltaTime;

        // 発射間隔に達したら
        if(bulletTimer >= bulletInterval)
        {
            // 弾を生成
            Instantiate(bulletPrefab, transform.position, Quaternion.identity);
            // タイマーリセット
            bulletTimer = 0.0f;
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