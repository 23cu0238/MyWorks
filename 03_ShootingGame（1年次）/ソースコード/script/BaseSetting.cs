// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// BaseSetting.cs
// ゲームの基本的な設定（フレームレートなど）を行うクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BaseSetting : MonoBehaviour
{
    // オブジェクトが有効化される前の、最初のフレーム更新前に呼び出されます
	private void Awake()
	{
		// 垂直同期をオフにする
		QualitySettings.vSyncCount = 0;
		// 目標フレームレートを60に設定
		Application.targetFrameRate = 60;
	}
}