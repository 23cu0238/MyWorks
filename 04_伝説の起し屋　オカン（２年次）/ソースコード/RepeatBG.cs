//---------------------------------------------------------------------------
//作成者：李朗曦
//作成日時：2024/10/18
//概要：タイトル画面の背景画像をスクロールさせるスクリプト
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class RepeatBG : MonoBehaviour
{
    [SerializeField] float mScrollSpeed;    //スクロール速度
    RawImage mImage;                        //スクロールした画像
    
    private void Start()
    {
        //代入
        mImage = GetComponent<RawImage>();
    }
    // Update is called once per frame
    void Update()
    {
        //画像のUVの頂点位置を移動
        mImage.uvRect = new Rect(mImage.uvRect.position + new Vector2(mScrollSpeed, mScrollSpeed), mImage.uvRect.size);
    }
}
