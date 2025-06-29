// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// LightSaber.cs
// サブ武器「ライトセーバー」の挙動を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor; // UnityEditor名前空間はビルド時にエラーになるため、通常は使用しないか、#if UNITY_EDITORで囲む

public class LightSaber : MonoBehaviour
{
    public float power; // ライトセーバーの攻撃力
    public float rotation; // 回転速度

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {

    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // メイン武器発射キー（左クリック）が押されている間
        if (Input.GetKey(KeyCode.Mouse0))
        {
            // Z軸周りに回転させる
            transform.Rotate(new Vector3(0, 0, rotation * Time.deltaTime));
        }
    }

    // 他のコライダーと接触した瞬間に呼ばれます
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // 敵またはボスにヒットした場合
        if (collision.gameObject.tag == "Enemy" || collision.gameObject.tag == "Boss")
        {
            // 相手にダメージを与える
            HP hit = collision.gameObject.GetComponent<HP>();
            if (hit != null)
            {
                hit.Damage(power);
            }
        }
    }
}