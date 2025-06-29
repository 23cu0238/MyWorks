// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// BeybladePlayerController.cpp
// �v���C���[�̓��͂���������R���g���[���[�̎���
// �쐬���F2025/04/28

#include "BeybladePlayerController.h"
#include "BeybladeBase.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// ���̃R���g���[���[�Ƀ}�b�s���O�R���e�L�X�g��ǉ����܂�
void ABeybladePlayerController::AddMappingContextForController()
{
    // ���[�J���v���C���[����Enhanced Input�T�u�V�X�e�����擾
    UEnhancedInputLocalPlayerSubsystem *InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    if (InputSubsystem && InputMappingContext)
    {
        // �}�b�s���O�R���e�L�X�g��ǉ�
        FModifyContextOptions Options;
        Options.bIgnoreAllPressedKeysUntilRelease = true;
        Options.bForceImmediately = false;
        Options.bNotifyUserSettings = false;
        InputSubsystem->AddMappingContext(InputMappingContext, 0, Options);
    }
}

// �Q�[���J�n���ɌĂ΂�܂�
void ABeybladePlayerController::BeginPlay()
{
    Super::BeginPlay();

    // �}�b�s���O�R���e�L�X�g��ǉ�
    AddMappingContextForController();
}

// ���t���[���Ă΂�܂�
void ABeybladePlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ���炩�̗��R�Ń}�b�s���O�R���e�L�X�g������ꂽ�ꍇ�A�ēx�ǉ������݂܂�
    if (!ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())->HasMappingContext(InputMappingContext))
    {
        AddMappingContextForController();
    }
}