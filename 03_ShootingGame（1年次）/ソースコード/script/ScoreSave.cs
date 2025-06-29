// 作成者：日本電子専門学校ゲーム制作研究科 23cu0238李朗曦
// ScoreSave.cs
// シーンを越えてスコア、残機、武器選択を保持するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ScoreSave : MonoBehaviour
{
    // シーン間で保持するデータ
    public int SavedScore; // 保存されたスコア
    public int Life; // 保存された残機
    public int MainWeaponIndex; // 保存されたメイン武器のインデックス
    public int SubWeaponIndex; // 保存されたサブ武器のインデックス

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    { 
        // このオブジェクトをシーン間で破壊されないように設定
        DontDestroyOnLoad(gameObject);
        // 武器インデックスを初期化
        MainWeaponIndex = -1;
        SubWeaponIndex = -1;
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        
    }

    // 保存されたスコアを取得する関数
    public int GetSavedScore()
    {
        return SavedScore;
    }

    // 保存された残機を取得する関数
    public int GetSavedLife()
    {
        return Life;
    }

    // スコアをクリア（0にリセット）する関数
    public void clrScore() 
    {
        SavedScore = 0;
    }

    // 残機をリセット（3に設定）する関数
    public void resetLife()
    {
        Life = 3;
    }

    // スコアを保存する関数
     public void SaveScore(int s)
    {
        SavedScore = s;
    }

    // 残機を保存する関数
    public void SaveLife(int l) 
    {
        Life = l;
    }
}