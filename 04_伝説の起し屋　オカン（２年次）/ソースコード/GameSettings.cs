//---------------------------------------------------------------------------
//�쐬�ҁF���N�F
//�쐬�����F2024/10/07
//�T�v�F�Q�[���̊�{�ݒ�
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameSettings : MonoBehaviour
{
    private void Awake()
    {
        //���������I�t�AFPS60�Ƀ��b�N
        QualitySettings.vSyncCount = 0;
        Application.targetFrameRate = 60;
    }
}
