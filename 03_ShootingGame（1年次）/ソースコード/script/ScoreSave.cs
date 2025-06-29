// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���� 23cu0238���N�F
// ScoreSave.cs
// �V�[�����z���ăX�R�A�A�c�@�A����I����ێ�����N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ScoreSave : MonoBehaviour
{
    // �V�[���Ԃŕێ�����f�[�^
    public int SavedScore; // �ۑ����ꂽ�X�R�A
    public int Life; // �ۑ����ꂽ�c�@
    public int MainWeaponIndex; // �ۑ����ꂽ���C������̃C���f�b�N�X
    public int SubWeaponIndex; // �ۑ����ꂽ�T�u����̃C���f�b�N�X

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    { 
        // ���̃I�u�W�F�N�g���V�[���ԂŔj�󂳂�Ȃ��悤�ɐݒ�
        DontDestroyOnLoad(gameObject);
        // ����C���f�b�N�X��������
        MainWeaponIndex = -1;
        SubWeaponIndex = -1;
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        
    }

    // �ۑ����ꂽ�X�R�A���擾����֐�
    public int GetSavedScore()
    {
        return SavedScore;
    }

    // �ۑ����ꂽ�c�@���擾����֐�
    public int GetSavedLife()
    {
        return Life;
    }

    // �X�R�A���N���A�i0�Ƀ��Z�b�g�j����֐�
    public void clrScore() 
    {
        SavedScore = 0;
    }

    // �c�@�����Z�b�g�i3�ɐݒ�j����֐�
    public void resetLife()
    {
        Life = 3;
    }

    // �X�R�A��ۑ�����֐�
     public void SaveScore(int s)
    {
        SavedScore = s;
    }

    // �c�@��ۑ�����֐�
    public void SaveLife(int l) 
    {
        Life = l;
    }
}