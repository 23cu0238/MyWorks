//---------------------------------------------------------------------------
//作成者：李朗曦
//作成日時：2024/10/07
//概要：ゲームの基本設定
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameSettings : MonoBehaviour
{
    private void Awake()
    {
        //垂直同期オフ、FPS60にロック
        QualitySettings.vSyncCount = 0;
        Application.targetFrameRate = 60;
    }
}
