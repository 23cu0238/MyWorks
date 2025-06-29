// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// BeamArrowLauncher.cs
// サブ武器「ビームアロー」の発射を管理するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BeamArrowLauncher : MonoBehaviour
{
    [SerializeField] GameObject BA; // ビームアローのプレハブ
    CD cD; // クールダウンタイマーの情報を管理するコンポーネント
    public float cooldown; // クールダウンの時間
    public float cdTimer; // 現在のクールダウンタイマー

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        cdTimer = 0; // クールダウンタイマーを初期化
        
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
            
            // サブ武器発射キー（右クリック）が押されたかチェック
            if (Input.GetKeyDown(KeyCode.Mouse1))
            {
                // シーン内のビームアロー（PlayerSubBulletタグ）を検索
                GameObject[] ba = GameObject.FindGameObjectsWithTag("PlayerSubBullet");

                // 設置済みのビームアローが5未満の場合
                if (ba.Length < 5)
                {
                    // 新しいビームアローを生成
                    Instantiate(BA, transform.position, Quaternion.identity);
                }
                // 設置済みのビームアローが5以上の場合
                else if (ba.Length >= 5)
                {
                    // 全ての設置済みビームアローを発射（アクティブ状態に）する
                    foreach (GameObject sc in ba)
                    {
                        if (sc != null) // 念のためnullチェック
                        {
                            sc.GetComponent<beamArrow>().mode = beamArrow.Mode.active;
                        }
                    }
                    // クールダウンタイマーを開始
                    cdTimer = cooldown;
                }
            }
        }

        // CDコンポーネントに現在のクールダウンタイマーを渡す
        if (cD != null)
        {
            cD.cd = cdTimer;
        }
    }
}