// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Explosion_Small.cs
// 小さい爆発アニメーションが終了したら自身を破棄するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Explosion_Small : MonoBehaviour
{
    Animator anim; // アニメーターコンポーネント

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        anim = GetComponent<Animator>();
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // 現在再生中のアニメーションが終了したら (normalizedTimeが1以上になったら)
        if (anim.GetCurrentAnimatorStateInfo(0).normalizedTime >= 1.0f)
        {
            // このゲームオブジェクトを破棄する
            Destroy(gameObject);
        }
    }
}