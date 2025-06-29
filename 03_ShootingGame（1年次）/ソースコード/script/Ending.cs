// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// Ending.cs
// �G���f�B���O�r�f�I�̍Đ��ƏI����̃V�[���J�ڂ𐧌䂷��N���X

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Video;
using UnityEngine.SceneManagement;

public class Ending : MonoBehaviour
{
    public VideoPlayer vid; // �r�f�I�v���C���[�R���|�[�l���g

    // �Q�[���J�n���Ɉ�x�����Ă΂�܂�
    void Start()
    {
        // �V�[�����UI�L�����o�X��j��
        if(GameObject.Find("Canvas") != null)
        {
            Destroy(GameObject.Find("Canvas"));
        }
        
        // �r�f�I�v���C���[�R���|�[�l���g���擾
        vid = GetComponent<VideoPlayer>();
        // �r�f�I�̍Đ����I�������Ƃ��ɌĂяo�����C�x���g��CheckOver�֐���o�^
        vid.loopPointReached += CheckOver;
    }

    // ���t���[���Ă΂�܂�
    void Update()
    {
        // �X�V�����͓��ɖ���
    }

    // �r�f�I�Đ��I�����ɌĂяo�����֐�
    void CheckOver(UnityEngine.Video.VideoPlayer vp)
    {
        // �X�e�[�W�N���A�V�[���ɑJ��
        SceneManager.LoadScene("StageEx_Clear");
    }
}