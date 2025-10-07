//---------------------------------------------------------------------------
//�쐬�ҁF���N�F
//�쐬�����F2024/10/20
//�T�v�F�^�C�g���V�[���𐧌䂷��}�l�[�W���[
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using Unity.Mathematics;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class TitleManager : MonoBehaviour
{
    [SerializeField] Text mPressAnyButtonText;  //�u�C�ӂ̃{�^���������Ă��������v�̕����I�u�W�F�N�g
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        //������_�ł�����
        mPressAnyButtonText.color = new Color(mPressAnyButtonText.color.r, mPressAnyButtonText.color.g,
            mPressAnyButtonText.color.b, Mathf.PingPong(Time.time,1));
        
        //�C�ӂ̃{�^���������ꂽ��v���C�V�[���ɐi��
        if(Input.anyKeyDown)
        {
            SceneManager.LoadSceneAsync("PlayScene");
        }
    }
}
