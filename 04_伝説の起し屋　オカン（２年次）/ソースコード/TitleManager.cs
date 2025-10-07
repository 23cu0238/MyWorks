//---------------------------------------------------------------------------
//作成者：李朗曦
//作成日時：2024/10/20
//概要：タイトルシーンを制御するマネージャー
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using Unity.Mathematics;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class TitleManager : MonoBehaviour
{
    [SerializeField] Text mPressAnyButtonText;  //「任意のボタンを押してください」の文字オブジェクト
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        //文字を点滅させる
        mPressAnyButtonText.color = new Color(mPressAnyButtonText.color.r, mPressAnyButtonText.color.g,
            mPressAnyButtonText.color.b, Mathf.PingPong(Time.time,1));
        
        //任意のボタンが押されたらプレイシーンに進む
        if(Input.anyKeyDown)
        {
            SceneManager.LoadSceneAsync("PlayScene");
        }
    }
}
