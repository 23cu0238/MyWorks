// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// armory.cs
// 武器選択画面（工廠）のUIとロジックを管理するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using TMPro;

public class armory : MonoBehaviour
{
    // コンポーネントとオブジェクトへの参照
    Camera MC; // メインカメラ
    Canvas canvas; // このスクリプトがアタッチされているキャンバス
    ScoreSave sc; // シーン間でデータを保持するScoreSaveオブジェクト
    public TMP_Dropdown DdMain; // メイン武器選択用のドロップダウン
    public TMP_Dropdown DdSub; // サブ武器選択用のドロップダウン

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // 必要なコンポーネントとオブジェクトを検索して取得
        MC = Camera.main;
        canvas = GetComponent<Canvas>();
        canvas.worldCamera = Camera.main; // キャンバスのレンダリングカメラを設定
        DdMain = GameObject.Find("DropdownMain").GetComponent<TMP_Dropdown>();
        DdSub = GameObject.Find("DropdownSub").GetComponent<TMP_Dropdown>();
        sc = GameObject.FindGameObjectWithTag("ScoreSave").GetComponent<ScoreSave>();
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // 毎フレーム、ドロップダウンで選択された武器情報を保存
        SaveWeaponOption();

        // Xキーが押されたらStage1に遷移
        if (Input.GetKeyDown(KeyCode.X))
        {
            SceneManager.LoadScene("Stage1");
        }
    }

    // 武器の選択情報をScoreSaveオブジェクトに保存する関数
    void SaveWeaponOption()
    {
        sc.MainWeaponIndex = DdMain.value; // メイン武器のインデックスを保存
        sc.SubWeaponIndex = DdSub.value;  // サブ武器のインデックスを保存
    }
}