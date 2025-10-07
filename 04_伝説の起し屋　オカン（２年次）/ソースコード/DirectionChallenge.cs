//---------------------------------------------------------------------------
//作成者：李朗曦
//作成日時：2024/10/16
//概要：アローズのミニゲーム派生クラス
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using UnityEditor;
using Unity.VisualScripting;

public class DirectionChallenge : ChallengesBase
{
    [SerializeField] GameObject mArrowPrefab;   //アローのプレハブ
    List<GameObject> mArrowList;                //生成されたアローリスト
    GameObject mIconCanvas;                     //アローの置き場

    bool bIsDPadReleased;   //コントローラの十字キーが押されていないかどうか

    // Start is called before the first frame update
    protected override void Start()
    {
        //基本クラスのスタートを呼び出す
        base.Start();

        //変数初期化
        bIsDPadReleased = true;
        mArrowList = new List<GameObject>();

        //初期代入
        mIconCanvas = transform.GetChild(2).gameObject;

        //アロー生成
        //このクラスのmChallengeAttributes=矢印の数
        for (int i = 0; i < (int)mChallengeAttributes; i++)
        {
            //生成し、リストに追加
            GameObject newArrow = Instantiate(mArrowPrefab, mIconCanvas.transform);
            mArrowList.Add(newArrow);

            //ランダムに生成
            //一つのプレハブのみ使用
            //回転と名前で差別化させる
            switch((int)Random.Range(0,4))
            {
                case 0:
                    newArrow.name = "Up";
                    newArrow.transform.localRotation = Quaternion.Euler(0, 0, -90) ;
                    break;

                case 1:
                    newArrow.name = "Down";
                    newArrow.transform.localRotation = Quaternion.Euler(0, 0, 90);
                    break;

                case 2:
                    newArrow.name = "Right";
                    newArrow.transform.localRotation = Quaternion.Euler(0, 0, 180);
                    break;

                case 3:
                    newArrow.name = "Left";
                    break;

            }

            //アローを置き場に放置し、中央揃える
            newArrow.transform.localPosition = new Vector3(
                (mChallengeAttributes / 2.0f) * -150.0f + 75.0f + i * 150.0f, 0);
        }
            
    }

    // Update is called once per frame
    protected override void Update()
    {
        //結果が出ていない時の処理
        if (mChallengeResult == ChallengeResult.none)
        {
            if((bIsDPadReleased &&  //十字キーが一回放たれたら
                (
                //WASDキー/十字キーが押されて、一番前のアローと合っている時の処理
                ((Input.GetButtonDown("ArrowUp") || Input.GetAxis("DPadY") > 0 )&& mArrowList[0].transform.name == "Up")
                || ((Input.GetButtonDown("ArrowLeft") || Input.GetAxis("DPadX") < 0) && mArrowList[0].transform.name == "Left")
                || ((Input.GetButtonDown("ArrowDown") || Input.GetAxis("DPadY") < 0) && mArrowList[0].transform.name == "Down")
                || ((Input.GetButtonDown("ArrowRight")|| Input.GetAxis("DPadX") > 0) && mArrowList[0].transform.name == "Right")
                )))
            {
                //十字キーが押されたことを記録
                bIsDPadReleased = false;
                //アローの処理
                ChangeArrow();
            }
            else if (    //WASDキー/十字キーが押されて、一番前のアローと合っていない時の処理
                Input.GetButtonDown("ArrowUp")
                || Input.GetButtonDown("ArrowLeft")
                || Input.GetButtonDown("ArrowDown")
                || Input.GetButtonDown("ArrowRight")
                || Input.GetAxis("DPadX") != 0
                || Input.GetAxis("DPadY") != 0
                )
            {
                //アローを赤くする
                mArrowList[0].GetComponent<SpriteRenderer>().color = Color.red;
                
                //結果を判定
                if (mArrowList.Count < (int)mChallengeAttributes / 2)   //半分以上のアローがあった時はgood
                {
                    mChallengeResult = ChallengeResult.good;
                }
                else　  //半分以下はbad
                {
                    mChallengeResult = ChallengeResult.bad;
                }
            }

            //十字キーが放たれたかどうかの判定
            if(Input.GetAxis("DPadX") == 0 && Input.GetAxis("DPadY") == 0)
            {
                bIsDPadReleased = true;
            }

            //全部正確に押されたらgreat
            if(mArrowList.Count <= 0)
            {
                mChallengeResult = ChallengeResult.great;
            }
        }

        //基本クラスの更新
        base.Update();
    }

    /// <summary>
    /// アローの色を緑にし、次のアローに目標変更
    /// </summary>
    private void ChangeArrow()
    {
        mArrowList[0].GetComponent<SpriteRenderer>().color = Color.green;
        mArrowList.RemoveAt(0);
    }

    /// <summary>
    /// デバッグ用
    /// </summary>
    void OnGUI()
    {
        if (Application.isEditor)  // or check the app debug flag
        {
            GUI.Label(new Rect(10, 10, 1000, 2000), bIsDPadReleased.ToString() 
                + "Y:"+Input.GetAxis("DPadY").ToString()
                + " X:" + Input.GetAxis("DPadX").ToString() + "\nJoystick Name:" + Input.GetJoystickNames()[0]);
        }
    }
}
