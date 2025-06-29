// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// BeybladePlayerController.cpp
// プレイヤーの入力を処理するコントローラーの実装
// 作成日：2025/04/28

#include "BeybladePlayerController.h"
#include "BeybladeBase.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// このコントローラーにマッピングコンテキストを追加します
void ABeybladePlayerController::AddMappingContextForController()
{
    // ローカルプレイヤーからEnhanced Inputサブシステムを取得
    UEnhancedInputLocalPlayerSubsystem *InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    if (InputSubsystem && InputMappingContext)
    {
        // マッピングコンテキストを追加
        FModifyContextOptions Options;
        Options.bIgnoreAllPressedKeysUntilRelease = true;
        Options.bForceImmediately = false;
        Options.bNotifyUserSettings = false;
        InputSubsystem->AddMappingContext(InputMappingContext, 0, Options);
    }
}

// ゲーム開始時に呼ばれます
void ABeybladePlayerController::BeginPlay()
{
    Super::BeginPlay();

    // マッピングコンテキストを追加
    AddMappingContextForController();
}

// 毎フレーム呼ばれます
void ABeybladePlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 何らかの理由でマッピングコンテキストが失われた場合、再度追加を試みます
    if (!ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())->HasMappingContext(InputMappingContext))
    {
        AddMappingContextForController();
    }
}