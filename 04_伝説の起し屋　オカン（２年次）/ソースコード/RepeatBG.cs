//---------------------------------------------------------------------------
//�쐬�ҁF���N�F
//�쐬�����F2024/10/18
//�T�v�F�^�C�g����ʂ̔w�i�摜���X�N���[��������X�N���v�g
//---------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class RepeatBG : MonoBehaviour
{
    [SerializeField] float mScrollSpeed;    //�X�N���[�����x
    RawImage mImage;                        //�X�N���[�������摜
    
    private void Start()
    {
        //���
        mImage = GetComponent<RawImage>();
    }
    // Update is called once per frame
    void Update()
    {
        //�摜��UV�̒��_�ʒu���ړ�
        mImage.uvRect = new Rect(mImage.uvRect.position + new Vector2(mScrollSpeed, mScrollSpeed), mImage.uvRect.size);
    }
}
