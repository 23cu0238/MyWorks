// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// cooldownTimer.cs
// サブ武器のクールダウン状態をUIのImageで視覚的に表示するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class cooldownTimer : MonoBehaviour
{
    Image image; // クールダウン表示用のUI Imageコンポーネント
    public CD sub; // サブ武器のクールダウン情報を保持するCDコンポーネント

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        image = GetComponent<Image>();
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // シーン内にサブ武器ランチャーが存在するか確認
        GameObject subWeaponLauncher = GameObject.FindGameObjectWithTag("SubWeaponLauncher");
        if (subWeaponLauncher != null)
        {
            // サブ武器ランチャーからCDコンポーネントを取得
            sub = subWeaponLauncher.GetComponent<CD>();
            if (sub != null && sub.CooldownAmount > 0)
            {
                // 残りクールダウン時間の割合を計算し、ImageのfillAmountに設定
                image.fillAmount = sub.cd / sub.CooldownAmount;
            }
            else
            {
                image.fillAmount = 0;
            }
        }
        else 
        {
            // サブ武器ランチャーがない場合はfillAmountを0にする
            image.fillAmount = 0;
        }
    }
}