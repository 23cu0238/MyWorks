// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// EnemyHP.cs
// 敵のHPをUIスライダー（HPバー）に反映させるクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class EnemyHP : MonoBehaviour
{
    Slider HB; // HPバーとして使用するSliderコンポーネント

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        HB = GetComponent<Slider>(); // 自分自身のSliderコンポーネントを取得
        
        // 親オブジェクトが持つHPコンポーネントから最大HPと現在HPを取得し、スライダーに設定
        HP parentHP = GetComponentInParent<HP>();
        if(parentHP != null)
        {
            HB.maxValue = parentHP.enemyHp;
            HB.value = parentHP.enemyHp;
        }
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // 親オブジェクトのHPを常にスライダーの値に反映させる
        HP parentHP = GetComponentInParent<HP>();
        if(parentHP != null)
        {
            HB.value = parentHP.enemyHp;
        }
        // 親が破壊された場合、このHPバーも非表示または破棄する処理を追加すると良い
        else
        {
            // 例: gameObject.SetActive(false);
        }
    }
}