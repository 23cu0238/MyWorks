// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// MainLaser.cs
// �v���C���[�̃��C������ł��郌�[�U�[�̕`��Ɠ����蔻��𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MainLaser: MonoBehaviour
{
    private LineRenderer line; // ���[�U�[�`��p��LineRenderer
    public float lineLength; // ���[�U�[�̒���
    public float lineWidth; // ���[�U�[�̕�
    // public LayerMask layerMask; // ���g�p�̂��߃R�����g�A�E�g
    PolygonCollider2D polygonCollider; // ���[�U�[�̓����蔻��p�̃|���S���R���C�_�[
    [SerializeField] Gradient[] _gradient; // �v���C���[�̃��[�h�ɉ������F�̃O���f�[�V�����z��

    [SerializeField] int[] power; // �v���C���[�̃��[�h�ɉ������U���́i�b�ԃ_���[�W�j
    Player player; // �v���C���[�R���|�[�l���g�ւ̎Q��

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        polygonCollider = GetComponent<PolygonCollider2D>();
        line = GetComponent<LineRenderer>();
        player = GameObject.FindGameObjectWithTag("Player").GetComponent<Player>();
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        if (player == null)
        {
            // �v���C���[�����Ȃ��ꍇ�͎��g��j��
            Destroy(gameObject);
            return;
        }
        
        // �v���C���[�̃��[�h�ɉ����ă��[�U�[�̐F��ݒ�
        if (_gradient.Length > player.mode)
        {
            line.colorGradient = _gradient[player.mode];
        }
        // ���[�U�[�̎n�_���v���C���[�̈ʒu�ɐݒ�
        line.SetPosition(0, player.transform.position);
        
        // ���C�����픭�˃L�[�i���N���b�N�j��������Ă����
        if (Input.GetKey(KeyCode.Mouse0))
        {
            // ���[�U�[��L�������A�I�_���v���C���[�̑O���ɐݒ�
            line.enabled = true;
            line.SetPosition(1, new Vector3(player.transform.position.x, player.transform.position.y + lineLength, 0));
        }
        else
        {
            // �L�[�������ꂽ�烌�[�U�[�𖳌���
            line.enabled = false;
            // �����蔻����������߂ɁA�I�_���n�_�Ɠ����ʒu�ɐݒ�
            line.SetPosition(1, player.transform.position);
        }
        
        // LineRenderer�̌`��ɍ��킹�ă|���S���R���C�_�[���X�V
        SetPolygonCollider(line);
    }

    // LineRenderer�̌`��Ɋ�Â���PolygonCollider2D�̒��_��ݒ肷��֐�
    void SetPolygonCollider(LineRenderer lineRenderer)
    {
        List<Vector2> point = new List<Vector2>();
        
        // ���[�U�[�̎n�_�ƏI�_����4�̒��_���v�Z
        point.Add(new Vector2(lineRenderer.GetPosition(0).x + lineWidth, lineRenderer.GetPosition(0).y));
        point.Add(new Vector2(lineRenderer.GetPosition(0).x - lineWidth, lineRenderer.GetPosition(0).y));
        point.Add(new Vector2(lineRenderer.GetPosition(1).x - lineWidth, lineRenderer.GetPosition(1).y));
        point.Add(new Vector2(lineRenderer.GetPosition(1).x + lineWidth, lineRenderer.GetPosition(1).y));

        // �R���C�_�[�̌`����X�V
        polygonCollider.SetPath(0, point);
    }

    // ���̃R���C�_�[�ƐڐG���Ă���ԁA���t���[���Ă΂�܂�
    private void OnTriggerStay2D(Collider2D collision)
    {
        // �ڐG���肪�G�܂��̓{�X�̏ꍇ
        if (collision.gameObject.tag == "Enemy" || collision.gameObject.tag == "Boss")
        {
            // ����Ɍp���_���[�W��^���� (Time.deltaTime����Z)
            HP hit = collision.gameObject.GetComponent<HP>();
            if (hit != null && player != null && power.Length > player.mode)
            {
                hit.Damage(power[player.mode] * Time.deltaTime);
            }
        }
    }
}