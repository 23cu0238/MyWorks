// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// DamageRED.cs
// ダメージを受けた際にスプライトを赤く点滅させるクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DamageRED : MonoBehaviour
{
    SpriteRenderer sr; // スプライト描画用コンポーネント
    public bool Change; // 色を変更中かどうかのフラグ
    public float REDTime; // 赤色表示の経過時間
    
    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        Change = false;
        REDTime = 0;
        sr = GetComponent<SpriteRenderer>();
    }

    // 毎フレーム呼ばれます
    private void Update()
    {
        // 色が変更中の場合
        if (Change == true)
        {
            REDTime += Time.deltaTime; // 経過時間を加算

            // 1秒以下の間、色を赤みがかった色に変更
            if (REDTime > 0 && REDTime <= 0.1f) // 持続時間を短く調整
            {
                sr.color = new Color(1f, 0.46f, 0.52f, 1f); // RGB値を0-1の範囲で指定
            }
            // 0.1秒を超えたら元の色に戻す
            else if (REDTime > 0.1f)
            {
                sr.color = Color.white; // new Color(1, 1, 1, 1) と同じ
                Change = false; // フラグをリセット
                REDTime = 0; // タイマーをリセット
            }
        }
    }

    // 他のコライダーと接触した瞬間に呼ばれます
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // プレイヤーの弾に当たった場合
        if (collision.gameObject.tag == "PlayerBullet") 
        {
            // 色変更フラグを立てる
            Change = true;
            REDTime = 0; // タイマーリセット
        }
    }
}