// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// xtc.cs
// TextMeshProUGUI�̃e�L�X�g��_�ł�����N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class xtc : MonoBehaviour
{
    // �_�ł�����e�L�X�g�R���|�[�l���g
    public TextMeshProUGUI text;

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // ���g�̃Q�[���I�u�W�F�N�g�ɃA�^�b�`����Ă���TextMeshProUGUI���擾
        text = GetComponent<TextMeshProUGUI>();
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �e�L�X�g�R���|�[�l���g���ݒ肳��Ă����
        if (text != null)
        {
            // Mathf.PingPong���g���āA�e�L�X�g�̃A���t�@�l(�����x)��0��1�̊Ԃŉ���������
            // ����ɂ��e�L�X�g���_�ł��Č�����
            text.color = new Color(1, 1, 1, Mathf.PingPong(Time.time, 1));
        }
    }
}