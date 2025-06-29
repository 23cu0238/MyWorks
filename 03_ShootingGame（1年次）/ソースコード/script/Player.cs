// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// Player.cs
// プレイヤーキャラクターの制御、武器管理、状態変化を担当するクラス

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    [SerializeField] GameObject[] MainPrefab; // メイン武器のプレハブ配列
    [SerializeField] GameObject[] SubPrefab;  // サブ武器のプレハブ配列
    // public GameObject doubleBulletPrefab; // 未使用
    // public GameObject laserPrefab; // 未使用
    // public GameObject EMPPrefab; // 未使用
    // public GameObject BAPrefab; // 未使用
    public int mode; // 現在のパワーアップモード (0:通常, 1:パワーアップ1, 2:パワーアップ2)
    float moveSpeed; // 現在の移動速度
    public float mode0Speed; // モード0の時の移動速度
    public float mode1Speed; // モード1の時の移動速度
    public float mode2Speed; // モード2の時の移動速度
    public GameObject explosionPrefab; // 破壊された時の爆発エフェクト
    public GameObject shieldPrefab; // 無敵時間中のシールドエフェクト
    public GameObject bfPrefab; // バックファイアエフェクト
    public int MainWeaponIndex; // 選択されたメイン武器のインデックス
    public int SubWeaponIndex; // 選択されたサブ武器のインデックス
    private BoxCollider2D PlayerCollider; // プレイヤーのコライダー
    public float powerUpTimer; // パワーアップの残り時間
    public float InvincibleTimer; // 無敵時間の残り時間
    // public float fireRate; // 未使用
    // float fireCountdown; // 未使用
    ScoreSave sc; // シーン間でデータを保持するオブジェクト

    // プレイヤーの状態
    public enum Status
    {
        Fine,
        Dead,
    };
    
    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        // ScoreSaveオブジェクトから武器選択情報を取得
        sc = GameObject.FindGameObjectWithTag("ScoreSave").GetComponent<ScoreSave>();
        mode = 0;
        MainWeaponIndex = sc.MainWeaponIndex;
        SubWeaponIndex = sc.SubWeaponIndex;
        
        // 各タイマーを初期化
        powerUpTimer = 0;
        InvincibleTimer = 2; // 開始時に2秒間の無敵時間

        // シールドとバックファイアエフェクトを生成し、自身の子オブジェクトにする
        Instantiate(shieldPrefab, transform.position, Quaternion.identity).transform.parent = gameObject.transform;
        Instantiate(bfPrefab, transform.position, Quaternion.identity).transform.parent = gameObject.transform;
        
        // 選択された武器を生成し、自身の子オブジェクトにする
        if(MainPrefab.Length > MainWeaponIndex && MainPrefab[MainWeaponIndex] != null)
            Instantiate(MainPrefab[MainWeaponIndex], transform.position, Quaternion.identity).transform.parent = gameObject.transform;
        if(SubPrefab.Length > SubWeaponIndex && SubPrefab[SubWeaponIndex] != null)
            Instantiate(SubPrefab[SubWeaponIndex], transform.position, Quaternion.identity).transform.parent = gameObject.transform;
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        // 無敵タイマーを減算
        InvincibleTimer -= Time.deltaTime;
        
        // === パワーアップ状態の管理 ===
        // ボスがおらず、パワーアップ時間が切れた場合
        if (GameObject.FindGameObjectWithTag("Boss") == null && powerUpTimer <= 0)
		{
            mode = 0;
            // レーザーなどの一部エフェクトを破棄（現在はプレイヤーに追従するため不要な可能性）
            // Destroy(GameObject.FindGameObjectWithTag("Laser"));
        }
        // ボスがおらず、パワーアップ時間が残っている場合
        else if(GameObject.FindGameObjectWithTag("Boss") == null && powerUpTimer > 0)
        {
            powerUpTimer -= Time.deltaTime;
        }
        // ボスがいる場合、強制的に最強モード(mode 2)にする
        else if (GameObject.FindGameObjectWithTag("Boss") != null)
        {
            mode = 2;
        }
        
        // === 移動処理 ===
        // マウスカーソルのワールド座標を取得
        Vector3 mousePosition = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        mousePosition.z = 0; // Z座標は0に固定
        // 移動範囲を制限
        mousePosition.x = Mathf.Clamp(mousePosition.x, -6.2f, 6.2f);
        mousePosition.y = Mathf.Clamp(mousePosition.y, -4.5f, 4.5f);
        
        // 現在位置からマウスカーソル位置へ滑らかに移動
        transform.position = Vector2.Lerp(transform.position, mousePosition, moveSpeed * Time.deltaTime); // Lerpの第三引数はTime.deltaTimeを乗算してフレームレート依存をなくす
        
        // === モードに応じた速度設定 ===
        if (mode == 0) Mode0();
        if (mode == 1) Mode1();
        if (mode >= 2) Mode2(); // 3以上は2として扱う
    }

    // 他のコライダーと接触した瞬間に呼ばれます
    private void OnTriggerEnter2D(Collider2D collision)
    {
        // 敵、敵弾、ボスと接触し、かつ無敵時間中でない場合
        if ((collision.gameObject.tag == "Enemy"|| collision.gameObject.tag == "EnemyBullet" || collision.gameObject.tag == "Boss") && InvincibleTimer <= 0)
        {
            // プレイヤーと関連オブジェクト（レーザー、弾など）を破棄
            Destroy(gameObject);
            GameObject laser = GameObject.FindGameObjectWithTag("Laser");
            if(laser != null) Destroy(laser);

            GameObject[] main = GameObject.FindGameObjectsWithTag("PlayerBullet");
            GameObject[] sub = GameObject.FindGameObjectsWithTag("PlayerSubBullet");
            foreach (GameObject mainObj in main) Destroy(mainObj);
            foreach (GameObject subObj in sub) Destroy(subObj);
            
            // 爆発エフェクトを生成
            Instantiate(explosionPrefab, transform.position, Quaternion.identity);
        }

        // アイテムと接触した場合
        if (collision.gameObject.tag == "Item")
        {
            Destroy(collision.gameObject);
            // モードが2未満なら1段階パワーアップ
            if (mode < 2)
            {
                mode += 1;
            }
            powerUpTimer = 10; // パワーアップ時間を10秒にセット
        }

        // ライフアイテムと接触した場合
        if (collision.gameObject.tag == "Life")
        {
            Destroy(collision.gameObject);
            // ゲームマネージャーにライフ追加を通知
            ShootingGameManager mng = GameObject.Find("Main Camera").GetComponent<ShootingGameManager>();
            mng.AddLife();
        }

        // EMPアイテムと接触した場合
        if (collision.gameObject.tag == "EMP")
        {
            Destroy(collision.gameObject);
            // EMP爆風を生成
            Instantiate(EMPPrefab, transform.position, Quaternion.identity);
        }
    }

    // モード0の時の設定
    void Mode0()
	{
        moveSpeed = mode0Speed;
	}

    // モード1の時の設定
    void Mode1()
    {
        moveSpeed = mode1Speed;
    }

    // モード2の時の設定
    void Mode2()
    {
        moveSpeed = mode2Speed;
    }
    
    // 現在の移動速度を返す関数（未使用）
    public float returnSpeed()
    {
        return moveSpeed;
    }
}