// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// MainLaser.cs
// プレイヤーのメイン武器であるレーザーの描画と当たり判定を制御するクラス

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MainLaser: MonoBehaviour
{
    private LineRenderer line; // レーザー描画用のLineRenderer
    public float lineLength; // レーザーの長さ
    public float lineWidth; // レーザーの幅
    // public LayerMask layerMask; // 未使用のためコメントアウト
    PolygonCollider2D polygonCollider; // レーザーの当たり判定用のポリゴンコライダー
    [SerializeField] Gradient[] _gradient; // プレイヤーのモードに応じた色のグラデーション配列

    [SerializeField] int[] power; // プレイヤーのモードに応じた攻撃力（秒間ダメージ）
    Player player; // プレイヤーコンポーネントへの参照

    // ゲーム開始時に一度だけ呼ばれます
    void Start()
    {
        polygonCollider = GetComponent<PolygonCollider2D>();
        line = GetComponent<LineRenderer>();
        player = GameObject.FindGameObjectWithTag("Player").GetComponent<Player>();
    }

    // 毎フレーム呼ばれます
    void Update()
    {
        if (player == null)
        {
            // プレイヤーがいない場合は自身を破棄
            Destroy(gameObject);
            return;
        }
        
        // プレイヤーのモードに応じてレーザーの色を設定
        if (_gradient.Length > player.mode)
        {
            line.colorGradient = _gradient[player.mode];
        }
        // レーザーの始点をプレイヤーの位置に設定
        line.SetPosition(0, player.transform.position);
        
        // メイン武器発射キー（左クリック）が押されている間
        if (Input.GetKey(KeyCode.Mouse0))
        {
            // レーザーを有効化し、終点をプレイヤーの前方に設定
            line.enabled = true;
            line.SetPosition(1, new Vector3(player.transform.position.x, player.transform.position.y + lineLength, 0));
        }
        else
        {
            // キーが離されたらレーザーを無効化
            line.enabled = false;
            // 当たり判定も消すために、終点を始点と同じ位置に設定
            line.SetPosition(1, player.transform.position);
        }
        
        // LineRendererの形状に合わせてポリゴンコライダーを更新
        SetPolygonCollider(line);
    }

    // LineRendererの形状に基づいてPolygonCollider2Dの頂点を設定する関数
    void SetPolygonCollider(LineRenderer lineRenderer)
    {
        List<Vector2> point = new List<Vector2>();
        
        // レーザーの始点と終点から4つの頂点を計算
        point.Add(new Vector2(lineRenderer.GetPosition(0).x + lineWidth, lineRenderer.GetPosition(0).y));
        point.Add(new Vector2(lineRenderer.GetPosition(0).x - lineWidth, lineRenderer.GetPosition(0).y));
        point.Add(new Vector2(lineRenderer.GetPosition(1).x - lineWidth, lineRenderer.GetPosition(1).y));
        point.Add(new Vector2(lineRenderer.GetPosition(1).x + lineWidth, lineRenderer.GetPosition(1).y));

        // コライダーの形状を更新
        polygonCollider.SetPath(0, point);
    }

    // 他のコライダーと接触している間、毎フレーム呼ばれます
    private void OnTriggerStay2D(Collider2D collision)
    {
        // 接触相手が敵またはボスの場合
        if (collision.gameObject.tag == "Enemy" || collision.gameObject.tag == "Boss")
        {
            // 相手に継続ダメージを与える (Time.deltaTimeを乗算)
            HP hit = collision.gameObject.GetComponent<HP>();
            if (hit != null && player != null && power.Length > player.mode)
            {
                hit.Damage(power[player.mode] * Time.deltaTime);
            }
        }
    }
}