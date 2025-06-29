// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// BossHoming.cs
// ボスが発射する追尾弾の挙動を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BossHoming : MonoBehaviour
{
    public float moveSpeed; // 弾の移動速度
    public GameObject player; // プレイヤーオブジェクトへの参照
    Rigidbody2D rigidBody; // Rigidbody2Dコンポーネント
    public float angleChangingSpeed; // 追尾時の旋回速度
    // float distance; // 未使用のためコメントアウト

    // 弾の行動モードを定義する列挙体
    public enum Mode
    {
        chasing, // 追尾モード
        stright  // 直進モード
    }

    public Mode mode; // 現在のモード

    // プレイヤーを見失った場合に使用する一時的な目標位置
    Vector2 tempPos;
    Vector2 NullPos = new Vector2(float.PositiveInfinity, float.PositiveInfinity); // 無効な位置を示す定数

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player");
        rigidBody = GetComponent<Rigidbody2D>();
        mode = Mode.chasing; // 初期モードは追尾
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // Z軸回転を固定する（2Dゲームでの意図しない回転を防ぐ）
        transform.rotation = new Quaternion(0f, 0f, transform.rotation.z, transform.rotation.w);

        // プレイヤーを毎フレーム検索
        player = GameObject.FindGameObjectWithTag("Player");
        
        // Y座標が1以下になったら、追尾をやめて直進モードに移行
        if(transform.position.y <= 1)
        { 
            mode = Mode.stright;
        }

        // 追尾モードの場合
        if (mode == Mode.chasing)
        {
            // プレイヤーが存在する場合
            if (player != null)
            {
                tempPos = NullPos; // 一時目標位置をリセット
                // プレイヤーへの方向ベクトルを計算
                Vector2 direction = (Vector2)player.transform.position - rigidBody.position;
                direction.Normalize();
                // 現在の前方ベクトルと目標方向との角度差を計算
                float rotateAmount = Vector3.Cross(direction, transform.up).z;
                // 角度差に応じて角速度を設定し、旋回させる
                rigidBody.angularVelocity = -angleChangingSpeed * rotateAmount;
                // 現在の前方方向に速度を設定
                rigidBody.velocity = transform.up * moveSpeed;
            }
            // プレイヤーが存在しない場合（撃破されたなど）
            else if (player == null)
            {
                // プレイヤーを見失った瞬間の位置を一時目標として保存
                if (tempPos == NullPos)
                {
                    tempPos = transform.position + transform.up * 10; // 暫定的に前方へ
                }
                // 一時目標に向かって移動
                Vector2 direction = tempPos - rigidBody.position;
                direction.Normalize();
                float rotateAmount = Vector3.Cross(direction, transform.up).z;
                rigidBody.angularVelocity = -angleChangingSpeed * rotateAmount;
                rigidBody.velocity = transform.up * moveSpeed;
            }
        }
        // 直進モードの場合
        else
        {
            // 旋回を停止し、現在の向きのまま直進
            rigidBody.angularVelocity = 0;
            rigidBody.velocity = transform.up * moveSpeed;
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