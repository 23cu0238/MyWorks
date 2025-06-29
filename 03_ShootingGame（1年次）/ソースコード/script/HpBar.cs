// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// HpBar.cs
// ボスのHPをUIのSliderとTextで表示するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class HpBar : MonoBehaviour
{
    Slider HB; // HPバー本体のSliderコンポーネント
    public Text BossHPtext; // HPをテキストで表示するUI Textコンポーネント

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        HB = GetComponent<Slider>();
        
        // シーン上のボスオブジェクトを探す
        GameObject boss = GameObject.FindGameObjectWithTag("Boss");
        if(boss != null)
        {
            // ボスのHPコンポーネントから最大HPを取得し、スライダーの最大値と現在値に設定
            HP bossHpComponent = boss.GetComponent<HP>();
            if(bossHpComponent != null)
            {
                HB.maxValue = bossHpComponent.enemyHp;
                HB.value = bossHpComponent.enemyHp;
                // テキストも更新
                BossHPtext.text = "Boss HP:" + (int)HB.value;
            }
        }
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // ボスオブジェクトを探す
        GameObject boss = GameObject.FindGameObjectWithTag("Boss");
        if(boss != null)
        {
            // ボスの現在のHPをスライダーとテキストに反映
            HP bossHpComponent = boss.GetComponent<HP>();
            if(bossHpComponent != null)
            {
                HB.value = bossHpComponent.enemyHp;
                BossHPtext.text = "Boss HP: " + (int)HB.value;
            }
        }
        else
        {
            // ボスがいない（倒された）場合、HPバーのUIを非表示にする
            gameObject.SetActive(false);
        }
    }
}