// 作成者：日本電子専門学校ゲーム制作研究科　23cu0238李朗曦
// BeybladeGameModeBase.cpp
// ゲームのルールを管理するゲームモードの実装
// 作成日：2025/04/29

#include "BeybladeGameModeBase.h"
#include "AIController.h"
#include "BeybladeGameInstance.h"
#include "BeybladePlayerController.h"
#include "EnvironmentManager.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "BeybladeBlueprintFunctionLibrary.h"

// コンストラクタ
ABeybladeGameModeBase::ABeybladeGameModeBase()
{
    // 毎フレームTickを有効化
    PrimaryActorTick.bCanEverTick = true;
    // デフォルトの起動オプション文字列
    OptionsString = FString("?Mode=CPU?1P=seiryu?2P=suzaku");
    // デフォルトポーンは使用しない
    DefaultPawnClass = nullptr;
    // プレイヤーコントローラークラスを設定
    PlayerControllerClass = ABeybladePlayerController::StaticClass();
}

// ゲーム開始時の処理
void ABeybladeGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // ゲーム状態を「プレイ中」に設定
    BeybladeGameState = EGameState::Playing;

    // オプション文字列が空の場合、デフォルト値を設定
    if (OptionsString == FString(""))
    {
        OptionsString = FString("?Mode=CPU?1P=seiryu?2P=suzaku");
        UE_LOG(LogTemp, Warning, TEXT("Option String: %s"), *OptionsString);
    }

    // オプション文字列からゲームモードを取得
    ModeStr = UGameplayStatics::ParseOption(OptionsString, "Mode");

    // ベイブレードのスポーン位置を設定
    BeybladeSpawnPos = {FVector(-65.0f, 0.0f, 129.0f), FVector(65.0f, 0.0f, 129.0f)};

    // ゲームインスタンスを取得し、キャスト
    GameInstance = Cast<UBeybladeGameInstance>(UGameplayStatics::GetGameInstance(this));

    // メインカメラを取得し、初期位置を保存
    MainCamera = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));
    if (MainCamera)
    {
        CameraOriginalPos = MainCamera->GetActorLocation(); // カメラの元の位置を保存
    }

    // 環境マネージャーをスポーンし、ゲームインスタンスに登録
    if (GameInstance)
    {
        EnvironmentManager = GetWorld()->SpawnActor<AEnvironmentManager>(AEnvironmentManager::StaticClass());
        GameInstance->EnvironmentManager = EnvironmentManager;
    }

    // 必殺技UIウィジェットを作成
    if (UltraWidgetClass)
    {
        UltraWidget = CreateWidget<UUserWidget>(GetWorld(), UltraWidgetClass, FName("Ultra Widget"));
    }

    if (UltraWidget)
    {
        // ウィジェットをビューポートに追加
        UltraWidget->AddToViewport(0);

        // ウィジェットを非表示に設定
        UltraWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    // ポーズUIウィジェットを作成
    if (PauseWidgetClass)
    {
        PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass, FName("Pause Widget"));
    }

    if (PauseWidget)
    {
        // ウィジェットをビューポートに追加
        PauseWidget->AddToViewport(0);

        // ウィジェットを非表示に設定
        PauseWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    // 入力デバイスの接続変更イベントにデリゲートをバインド
    if (GameInstance)
    {
        FScriptDelegate Delegate;
        Delegate.BindUFunction(this, FName("OnInputDeviceConnectionChange"));
        GameInstance->OnInputDeviceConnectionChange.AddUnique(Delegate);
        GameInstance->BeybladeList.Empty(); // ベイブレードリストを初期化
    }

    // マルチプレイヤーモードの場合の初期設定
    if (IsMultiplayer)
    {
        // 一度だけ実行
        static bool bHasExecuted = false;
        if (!bHasExecuted)
        {
            bHasExecuted = true;
            // 2P用のローカルプレイヤーを作成
            APlayerController *NewPlayerController = UGameplayStatics::CreatePlayer(GetWorld(), -1, true);
        }
    }

    // プレイヤー1と2のベイブレードを生成
    for (int32 Index = 1; Index <= 2; ++Index)
    {
        FString PlayerKey = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Conv_IntToString(Index), TEXT("P")); // "1P" または "2P"
        FString BeybladeNameSelectedStr = UGameplayStatics::ParseOption(OptionsString, PlayerKey);                         // オプション文字列からベイブレード名を取得
        SpawnedBeyblade = nullptr;                                                                                         // 各ループの開始時にリセット

        if (BeybladeNameSelectedStr.IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("No Beyblade selection found for %s in OptionsString: %s. Check game startup options."), *PlayerKey, *OptionsString);
            // フォールバックとしてデフォルトのベイブレードをスポーンするか、エラー処理を行う

            // BeybladeClassMapから最初の有効なエントリを使用する
            if (!BeybladeClassMap.IsEmpty())
            {
                for (const auto &Pair : BeybladeClassMap)
                {
                    if (Pair.Value != nullptr)
                    {
                        BeybladeNameSelectedStr = Pair.Key.ToString();
                        UE_LOG(LogTemp, Warning, TEXT("Falling back to default Beyblade: %s for %s"), *BeybladeNameSelectedStr, *PlayerKey);
                        break;
                    }
                }
            }
            if (BeybladeNameSelectedStr.IsEmpty()) // それでも空ならスキップ
            {
                UE_LOG(LogTemp, Error, TEXT("No default Beyblade available for %s. Skipping spawn."), *PlayerKey);
                continue;
            }
        }

        // 安全な配列アクセスのために、インデックスが有効かチェック
        if (!BeybladeSpawnPos.IsValidIndex(Index - 1))
        {
            UE_LOG(LogTemp, Error, TEXT("Spawn failed for player %d: BeybladeSpawnPos is invalid for index %d."), Index, Index - 1);
            continue; // このプレイヤーのスポーン処理をスキップ
        }

        FName BeybladeNameSelected = FName(*BeybladeNameSelectedStr); // 選択されたベイブレード名をFNameに変換

        FVector SpawnLocation = BeybladeSpawnPos[Index - 1]; // スポーン位置を取得
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined; // 衝突処理を未定義に設定

        // ベイブレードクラスをBeybladeClassMapから取得
        TSubclassOf<ABeybladeBase> *FoundClassPtr = BeybladeClassMap.Find(BeybladeNameSelected);

        // クラスが見つかった場合、スポーンを試みる
        if (FoundClassPtr && *FoundClassPtr != nullptr)
        {
            SpawnedBeyblade = GetWorld()->SpawnActor<ABeybladeBase>(*FoundClassPtr, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

            // スポーンが成功したか確認
            if (!SpawnedBeyblade)
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to spawn Beyblade '%s' for %s despite class being found."), *BeybladeNameSelected.ToString(), *PlayerKey);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Beyblade class for name '%s' (requested by %s) not found in BeybladeClassMap or is null. OptionsString: %s"), *BeybladeNameSelected.ToString(), *PlayerKey, *OptionsString); // クラスが見つからない場合のエラーログ
        }

        // スポーンしたベイブレードが有効であれば、ゲームインスタンスのリストに追加
        if (SpawnedBeyblade && GameInstance)
        {
            GameInstance->BeybladeList.Add(Index - 1, SpawnedBeyblade); // インデックスを0から始めるためにIndex - 1を使用
        }
        else if (!SpawnedBeyblade)
        {
            UE_LOG(LogTemp, Error, TEXT("Beyblade for player %d was not spawned. Cannot add to list or possess."), Index);
            continue; // このプレイヤーの残りの処理をスキップ
        }

        if (ModeStr == "CPU" && Index == 2)
        {
            // --- AIコントローラーを手動でスポーン＆ポゼッション ---

            // スポーンされたベイブレードがデフォルトで使用するべきAIコントローラーのクラスを取得
            TSubclassOf<AController> AIControllerClass = SpawnedBeyblade->AIControllerClass;
            if (AIControllerClass)
            {
                // AIコントローラーをスポーン
                AAIController *AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnedBeyblade->GetActorTransform());
                if (AIController)
                {
                    // スポーンしたAIコントローラーでベイブレードをポゼッション
                    AIController->Possess(SpawnedBeyblade);
                    UE_LOG(LogTemp, Log, TEXT("Manually spawned and possessed AI Controller for %s"), *SpawnedBeyblade->GetName());
                }
            }
        }
        else
        {
            // --- 従来のプレイヤーコントローラーのポゼッションロジック ---
            int possesIndex = (Index == 1) ? 0 : Index - 1;
            APlayerController *PlayerController = UGameplayStatics::GetPlayerController(this, possesIndex);
            if (PlayerController)
            {
                PlayerController->Possess(SpawnedBeyblade);
            }
        }
    }
}

// 毎フレームの処理
void ABeybladeGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!MainCamera || !GameInstance)
        return;

    // 現在のカメラ位置を取得
    FVector CameraLocation = MainCamera->GetActorLocation();

    // ゲームインスタンスからベイブレードリストを取得
    TMap<int32, ABeybladeBase *> BeybladeList = GameInstance->BeybladeList;

    // マウスカーの表示状態を更新
    if (BeybladeGameState == EGameState::Playing)
    {
        UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
    }

    // ベイブレードリストの要素が1つになったら（＝勝者が決まったら）
    if (BeybladeList.Num() == 1)
    {
        // ゲーム状態を「終了」に設定
        BeybladeGameState = EGameState::Finished;

        // リストの最初の（唯一の）要素を取得
        TMap<int32, ABeybladeBase *>::TConstIterator BeybladeListConstIterator = BeybladeList.CreateConstIterator();
        ABeybladeBase *FirstBeyblade = BeybladeListConstIterator.Value();

        // 勝者へのズームアップカメラ位置を計算
        FVector NewLocation = UKismetMathLibrary::VLerp(CameraLocation, FirstBeyblade->GetActorLocation() + FVector(-15.0f, 0.0f, 30.0f), 0.2f);

        // カメラを新しい位置へ移動
        MainCamera->SetActorLocation(NewLocation, false, nullptr, ETeleportType::None);

        // 勝者インデックスを決定
        int WinnerIndex = (FirstBeyblade->GetControllerType() == EBeybladeControllerType::P1) ? 1 : 2;

        // 終了ウィジェットを作成して表示
        if (FinishWidgetClass)
        {
            if (!FinishWidget)
            {
                FinishWidget = CreateWidget<UUserWidget>(GetWorld(), FinishWidgetClass, FName("Finish Widget"));
            }
            // ウィジェットの初期化関数を呼び出す
            UFunction *InitFunc = FinishWidget->FindFunction(FName("InitFinishWidget"));
            if (InitFunc)
            {
                FinishWidget->ProcessEvent(InitFunc, &WinnerIndex);
            }
        }

        if (FinishWidget && !FinishWidget->IsInViewport())
        {
            FinishWidget->AddToViewport(0);
        }
    }

    // 必殺技のライトエフェクトがオンの場合
    if (EnvironmentManager->GetIsUltraLightOn())
    {
        ABeybladeBase *UltimateUser = GameInstance->GetUltimateUser();
        if (UltimateUser)
        {
            // 必殺技使用者を追うようにカメラを移動
            FVector UltimateUserLocation = UltimateUser->GetActorLocation();
            FVector NewLocation = UKismetMathLibrary::VLerp(CameraLocation, UltimateUserLocation + FVector(-50.0f, 0.0f, 100.0f), 0.05f);

            MainCamera->SetActorLocation(NewLocation, false, nullptr, ETeleportType::None);

            // 必殺技UIを表示
            UltraWidget->SetVisibility(ESlateVisibility::HitTestInvisible);

            // スポットライトを必殺技使用者の上に移動
            EnvironmentManager->MoveSpotlight(UltimateUserLocation + FVector(0.0f, 0.0f, 300.0f));
        }
    }
    // 通常時（ベイブレードが2体以上）のカメラ処理
    else if (BeybladeList.Num() > 1)
    {
        // カメラを元の位置に滑らかに戻す
        FVector NewLocation = UKismetMathLibrary::VLerp(CameraLocation, CameraOriginalPos, 0.05f);
        MainCamera->SetActorLocation(NewLocation, false, nullptr, ETeleportType::None);

        // 必殺技UIを非表示
        UltraWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (BeybladeGameState == EGameState::Finished)
    {
        GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
        // ゲームが終了したらマウスカーソルを表示
    }
}

// 入力デバイスの接続状態が変更されたときの処理
void ABeybladeGameModeBase::OnInputDeviceConnectionChange(EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId)
{
    FString InputDeviceIdStr = UKismetStringLibrary::Conv_InputDeviceIdToString(InputDeviceId);
    FString PlatformUserIdStr = UKismetStringLibrary::Conv_PlatformUserIdToString(PlatformUserId);

    // 2Pコントローラーの接続に関連すると思われる処理
    // IDが"1"のデバイスかつプラットフォームIDが"1"の場合
    bool bIsInputDeviceIdOne = UKismetStringLibrary::EqualEqual_StrStr(InputDeviceIdStr, TEXT("1"));
    bool bIsPlatformUserIdOne = UKismetStringLibrary::EqualEqual_StrStr(PlatformUserIdStr, TEXT("1"));

    if (bIsInputDeviceIdOne && bIsPlatformUserIdOne)
    {
        // 接続状態に応じてゲームパッドIDのオフセットを設定
        switch (NewConnectionState)
        {
        case EInputDeviceConnectionState::Disconnected:
            UBeybladeBlueprintFunctionLibrary::OffsetGamepadID(false);
            break;

        case EInputDeviceConnectionState::Connected:
            UBeybladeBlueprintFunctionLibrary::OffsetGamepadID(true);
            break;

        default:
            break;
        }
    }
}

// ポーズ状態の切り替え
void ABeybladeGameModeBase::TogglePause(int32 PlayerIndexWhoRequestedPause)
{
    // プレイング状態からポーズ状態に切り替える場合
    if (BeybladeGameState == EGameState::Playing)
    {
        BeybladeGameState = EGameState::Paused;
        PausedPlayerIndex = PlayerIndexWhoRequestedPause; // ポーズしたプレイヤーを記録
        UGameplayStatics::SetGamePaused(GetWorld(), true);
        if (PauseWidget)
        {
            // マウス表示
            APlayerController *Player0Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (Player0Controller)
            {
                Player0Controller->SetShowMouseCursor(true);
            }
            PauseWidget->SetVisibility(ESlateVisibility::Visible);
        }
    }
    else if (BeybladeGameState == EGameState::Paused)
    {
        /*ポーズをかけたプレイヤー本人のみ解除可能*/
        if (PlayerIndexWhoRequestedPause != PausedPlayerIndex && PausedPlayerIndex != -1)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player %d cannot unpause, game was paused by Player %d"), PlayerIndexWhoRequestedPause, PausedPlayerIndex);
            return;
        }

        BeybladeGameState = EGameState::Playing;
        PausedPlayerIndex = -1; // リセット
        UGameplayStatics::SetGamePaused(GetWorld(), false);
        if (PauseWidget)
        {
            PauseWidget->SetVisibility(ESlateVisibility::Hidden);
            // マウス表示
            APlayerController *Player0Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (Player0Controller)
            {
                Player0Controller->SetShowMouseCursor(false);
            }
        }
    }
}