// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// powerUpIconMain.cs
// 武器選択画面で、メイン武器のドロップダウン選択に応じてアイコンを表示するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class powerUpIconMain : MonoBehaviour
{
    SpriteRenderer sr; // このオブジェクト自身のSpriteRenderer (未使用の可能性)
    SpriteRenderer IconSr; // アイコン表示用のSpriteRenderer
    public TMP_Dropdown DdMain; // メイン武器選択用のドロップダウン
    armory arm; // 親オブジェクトのarmoryスクリプト

    [SerializeField] Sprite[] sprite; // ドロップダウンの選択肢に対応するスプライトの配列

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        sr = GetComponent<SpriteRenderer>();
        // シーン内の"IconMain"オブジェクトからSpriteRendererを取得
        IconSr = GameObject.Find("IconMain").GetComponent<SpriteRenderer>();
        // 子オブジェクトからドロップダウンコンポーネントを取得
        DdMain = GetComponentInChildren<TMP_Dropdown>();
        // 親オブジェクトからarmoryコンポーネントを取得
        arm = GetComponentInParent<armory>();
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // ドロップダウンの現在の選択値(value)をインデックスとして、対応するスプライトをアイコンに設定
        if (IconSr != null && DdMain != null && sprite.Length > DdMain.value)
        {
            IconSr.sprite = sprite[DdMain.value];
        }
    }
}