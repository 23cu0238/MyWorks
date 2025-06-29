// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// MoveTowardsPlayer.cs
// プレイヤーに向かって移動する敵の挙動を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveTowardsPlayer : MonoBehaviour
{
    public float moveSpeed; // 移動速度
    GameObject player; // プレイヤーオブジェクトへの参照

    float distance; // プレイヤーとの距離

    // 敵の行動モード
    public enum Mode
    {
        initial, // 初期位置への移動フェーズ
        moving,  // プレイヤーへの追跡フェーズ
    };

    public Mode mode; // 現在のモード
    float timer; // モード移行用のタイマー

    public Vector3 startPos; // 初期移動の目標位置

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player");
        mode = Mode.initial;
        
        // 画面上部、自身のX座標付近にランダムな初期目標位置を設定
        Vector2 randPos = Random.insideUnitCircle * 1;
        if (randPos.x < -6.2f) randPos.x = -6.2f;
        if (randPos.x > 6.2f) randPos.x = 6.2f;
        startPos = new Vector3(randPos.x + transform.position.x, 4.5f, 0); // Y座標を画面上部に修正
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        timer += Time.deltaTime;
        // 1秒後に追跡モードに移行
        if (timer >= 1)
        {
            mode = Mode.moving;
        }

        // プレイヤーがいない場合は何もしない
        if (GameObject.FindGameObjectWithTag("Player") == null)
        {
            // プレイヤーがいない場合、その場で停止するなどの挙動を追加してもよい
            return;
        }
        else
        {
            player = GameObject.FindGameObjectWithTag("Player");
        }
        
        // プレイヤーとの距離を計算
        distance = Vector3.Distance(transform.position, player.transform.position);

        // 追跡モードの場合
        if(mode == Mode.moving)
        { 
            // プレイヤーとの距離が近い場合、速度を上げて追跡
            if (distance <= 2.0f)
            {
                transform.position = Vector2.Lerp(transform.position, player.transform.position, moveSpeed * 2 * Time.deltaTime);
            }
            // プレイヤーとの距離が遠い場合、真下に移動
            if (distance > 2.0f)
            {
                transform.Translate(0.0f, -moveSpeed * Time.deltaTime, 0.0f);
            } 
        }
        // 初期移動フェーズの場合
        else
        {
            // 設定された初期目標位置に向かって移動
            Vector2 dir = (Vector2)startPos - (Vector2)transform.position;
            transform.Translate(dir.normalized * 2 * Time.deltaTime, Space.World);
        }
    }
}