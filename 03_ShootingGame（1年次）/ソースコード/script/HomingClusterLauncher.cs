// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// HomingClusterLauncher.cs
// サブ武器「ホーミングクラスター」の発射を管理するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HomingClusterLauncher : MonoBehaviour
{
    [SerializeField] GameObject HC; // ホーミングクラスターのプレハブ
    CD cD; // クールダウン情報を管理するコンポーネント
    public int ClusterNum; // 一度に発射するミサイルの数
    public float cooldown; // クールダウン時間
    public float cdTimer; // 現在のクールダウンタイマー

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        cdTimer = 0; // タイマー初期化
        
        // CDコンポーネントを取得し、クールダウン時間を設定
        cD = GetComponent<CD>();
        if (cD != null)
        {
            cD.CooldownAmount = cooldown;
        }
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // クールダウンタイマーを減算
        if (cdTimer > 0)
        {
            cdTimer -= Time.deltaTime;
        }
        else if (cdTimer <= 0)
        {
            cdTimer = 0; // マイナスにならないように調整
            
            // サブ武器発射キー（右クリック）が押されている間
            if (Input.GetKey(KeyCode.Mouse1))
            {
                // 指定された数のミサイルを一度に生成
                for (int i = 0; i < ClusterNum; ++i)
                {
                    Instantiate(HC, transform.position, Quaternion.identity);
                }
                // クールダウンタイマーを開始
                cdTimer = cooldown;
            }
        }

        // 現在のクールダウンタイマーをCDコンポーネントに渡す
        if (cD != null)
        {
            cD.cd = cdTimer;
        }
    }
}