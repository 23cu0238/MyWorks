// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Ending.cs
// エンディングビデオの再生と終了後のシーン遷移を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Video;
using UnityEngine.SceneManagement;

public class Ending : MonoBehaviour
{
    public VideoPlayer vid; // ビデオプレイヤーコンポーネント

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // シーン上のUIキャンバスを破棄
        if(GameObject.Find("Canvas") != null)
        {
            Destroy(GameObject.Find("Canvas"));
        }
        
        // ビデオプレイヤーコンポーネントを取得
        vid = GetComponent<VideoPlayer>();
        // ビデオの再生が終了したときに呼び出されるイベントにCheckOver関数を登録
        vid.loopPointReached += CheckOver;
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // 更新処理は特に無し
    }

    // ビデオ再生終了時に呼び出される関数
    void CheckOver(UnityEngine.Video.VideoPlayer vp)
    {
        // ステージクリアシーンに遷移
        SceneManager.LoadScene("StageEx_Clear");
    }
}