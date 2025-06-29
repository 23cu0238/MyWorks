// 作成者：日本電子専門学校ゲーム制作研究科 23cu0238李朗曦
// scanner.cs
// 特定のオブジェクト（親）に追従する機能を持つクラス（現状は不完全）

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class scanner : MonoBehaviour
{
    GameObject parents; // 親オブジェクト
    MoveTowardsPlayer MTP; // (未使用)

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // Start内で親オブジェクトを設定するロジックが必要
    }

    // 毎フレーム呼ばれます
    void Update()
    {
       // 親オブジェクトが設定されていれば、その位置に追従する
        if (parents != null)
        {
            transform.position = parents.transform.position;
        }
    }
}