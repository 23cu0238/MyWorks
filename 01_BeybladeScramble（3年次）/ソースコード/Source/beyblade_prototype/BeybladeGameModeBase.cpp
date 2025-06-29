// �쐬�ҁF���{�d�q���w�Z�Q�[�����쌤���ȁ@23cu0238���N�F
// BeybladeGameModeBase.cpp
// �Q�[���̃��[�����Ǘ�����Q�[�����[�h�̎���
// �쐬���F2025/04/29

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

// �R���X�g���N�^
ABeybladeGameModeBase::ABeybladeGameModeBase()
{
    // ���t���[��Tick��L����
    PrimaryActorTick.bCanEverTick = true;
    // �f�t�H���g�̋N���I�v�V����������
    OptionsString = FString("?Mode=CPU?1P=seiryu?2P=suzaku");
    // �f�t�H���g�|�[���͎g�p���Ȃ�
    DefaultPawnClass = nullptr;
    // �v���C���[�R���g���[���[�N���X��ݒ�
    PlayerControllerClass = ABeybladePlayerController::StaticClass();
}

// �Q�[���J�n���̏���
void ABeybladeGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // �Q�[����Ԃ��u�v���C���v�ɐݒ�
    BeybladeGameState = EGameState::Playing;

    // �I�v�V���������񂪋�̏ꍇ�A�f�t�H���g�l��ݒ�
    if (OptionsString == FString(""))
    {
        OptionsString = FString("?Mode=CPU?1P=seiryu?2P=suzaku");
        UE_LOG(LogTemp, Warning, TEXT("Option String: %s"), *OptionsString);
    }

    // �I�v�V���������񂩂�Q�[�����[�h���擾
    ModeStr = UGameplayStatics::ParseOption(OptionsString, "Mode");

    // �x�C�u���[�h�̃X�|�[���ʒu��ݒ�
    BeybladeSpawnPos = {FVector(-65.0f, 0.0f, 129.0f), FVector(65.0f, 0.0f, 129.0f)};

    // �Q�[���C���X�^���X���擾���A�L���X�g
    GameInstance = Cast<UBeybladeGameInstance>(UGameplayStatics::GetGameInstance(this));

    // ���C���J�������擾���A�����ʒu��ۑ�
    MainCamera = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));
    if (MainCamera)
    {
        CameraOriginalPos = MainCamera->GetActorLocation(); // �J�����̌��̈ʒu��ۑ�
    }

    // ���}�l�[�W���[���X�|�[�����A�Q�[���C���X�^���X�ɓo�^
    if (GameInstance)
    {
        EnvironmentManager = GetWorld()->SpawnActor<AEnvironmentManager>(AEnvironmentManager::StaticClass());
        GameInstance->EnvironmentManager = EnvironmentManager;
    }

    // �K�E�ZUI�E�B�W�F�b�g���쐬
    if (UltraWidgetClass)
    {
        UltraWidget = CreateWidget<UUserWidget>(GetWorld(), UltraWidgetClass, FName("Ultra Widget"));
    }

    if (UltraWidget)
    {
        // �E�B�W�F�b�g���r���[�|�[�g�ɒǉ�
        UltraWidget->AddToViewport(0);

        // �E�B�W�F�b�g���\���ɐݒ�
        UltraWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    // �|�[�YUI�E�B�W�F�b�g���쐬
    if (PauseWidgetClass)
    {
        PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass, FName("Pause Widget"));
    }

    if (PauseWidget)
    {
        // �E�B�W�F�b�g���r���[�|�[�g�ɒǉ�
        PauseWidget->AddToViewport(0);

        // �E�B�W�F�b�g���\���ɐݒ�
        PauseWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    // ���̓f�o�C�X�̐ڑ��ύX�C�x���g�Ƀf���Q�[�g���o�C���h
    if (GameInstance)
    {
        FScriptDelegate Delegate;
        Delegate.BindUFunction(this, FName("OnInputDeviceConnectionChange"));
        GameInstance->OnInputDeviceConnectionChange.AddUnique(Delegate);
        GameInstance->BeybladeList.Empty(); // �x�C�u���[�h���X�g��������
    }

    // �}���`�v���C���[���[�h�̏ꍇ�̏����ݒ�
    if (IsMultiplayer)
    {
        // ��x�������s
        static bool bHasExecuted = false;
        if (!bHasExecuted)
        {
            bHasExecuted = true;
            // 2P�p�̃��[�J���v���C���[���쐬
            APlayerController *NewPlayerController = UGameplayStatics::CreatePlayer(GetWorld(), -1, true);
        }
    }

    // �v���C���[1��2�̃x�C�u���[�h�𐶐�
    for (int32 Index = 1; Index <= 2; ++Index)
    {
        FString PlayerKey = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Conv_IntToString(Index), TEXT("P")); // "1P" �܂��� "2P"
        FString BeybladeNameSelectedStr = UGameplayStatics::ParseOption(OptionsString, PlayerKey);                         // �I�v�V���������񂩂�x�C�u���[�h�����擾
        SpawnedBeyblade = nullptr;                                                                                         // �e���[�v�̊J�n���Ƀ��Z�b�g

        if (BeybladeNameSelectedStr.IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("No Beyblade selection found for %s in OptionsString: %s. Check game startup options."), *PlayerKey, *OptionsString);
            // �t�H�[���o�b�N�Ƃ��ăf�t�H���g�̃x�C�u���[�h���X�|�[�����邩�A�G���[�������s��

            // BeybladeClassMap����ŏ��̗L���ȃG���g�����g�p����
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
            if (BeybladeNameSelectedStr.IsEmpty()) // ����ł���Ȃ�X�L�b�v
            {
                UE_LOG(LogTemp, Error, TEXT("No default Beyblade available for %s. Skipping spawn."), *PlayerKey);
                continue;
            }
        }

        // ���S�Ȕz��A�N�Z�X�̂��߂ɁA�C���f�b�N�X���L�����`�F�b�N
        if (!BeybladeSpawnPos.IsValidIndex(Index - 1))
        {
            UE_LOG(LogTemp, Error, TEXT("Spawn failed for player %d: BeybladeSpawnPos is invalid for index %d."), Index, Index - 1);
            continue; // ���̃v���C���[�̃X�|�[���������X�L�b�v
        }

        FName BeybladeNameSelected = FName(*BeybladeNameSelectedStr); // �I�����ꂽ�x�C�u���[�h����FName�ɕϊ�

        FVector SpawnLocation = BeybladeSpawnPos[Index - 1]; // �X�|�[���ʒu���擾
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined; // �Փˏ����𖢒�`�ɐݒ�

        // �x�C�u���[�h�N���X��BeybladeClassMap����擾
        TSubclassOf<ABeybladeBase> *FoundClassPtr = BeybladeClassMap.Find(BeybladeNameSelected);

        // �N���X�����������ꍇ�A�X�|�[�������݂�
        if (FoundClassPtr && *FoundClassPtr != nullptr)
        {
            SpawnedBeyblade = GetWorld()->SpawnActor<ABeybladeBase>(*FoundClassPtr, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

            // �X�|�[���������������m�F
            if (!SpawnedBeyblade)
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to spawn Beyblade '%s' for %s despite class being found."), *BeybladeNameSelected.ToString(), *PlayerKey);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Beyblade class for name '%s' (requested by %s) not found in BeybladeClassMap or is null. OptionsString: %s"), *BeybladeNameSelected.ToString(), *PlayerKey, *OptionsString); // �N���X��������Ȃ��ꍇ�̃G���[���O
        }

        // �X�|�[�������x�C�u���[�h���L���ł���΁A�Q�[���C���X�^���X�̃��X�g�ɒǉ�
        if (SpawnedBeyblade && GameInstance)
        {
            GameInstance->BeybladeList.Add(Index - 1, SpawnedBeyblade); // �C���f�b�N�X��0����n�߂邽�߂�Index - 1���g�p
        }
        else if (!SpawnedBeyblade)
        {
            UE_LOG(LogTemp, Error, TEXT("Beyblade for player %d was not spawned. Cannot add to list or possess."), Index);
            continue; // ���̃v���C���[�̎c��̏������X�L�b�v
        }

        if (ModeStr == "CPU" && Index == 2)
        {
            // --- AI�R���g���[���[���蓮�ŃX�|�[�����|�[�b�V���� ---

            // �X�|�[�����ꂽ�x�C�u���[�h���f�t�H���g�Ŏg�p����ׂ�AI�R���g���[���[�̃N���X���擾
            TSubclassOf<AController> AIControllerClass = SpawnedBeyblade->AIControllerClass;
            if (AIControllerClass)
            {
                // AI�R���g���[���[���X�|�[��
                AAIController *AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnedBeyblade->GetActorTransform());
                if (AIController)
                {
                    // �X�|�[������AI�R���g���[���[�Ńx�C�u���[�h���|�[�b�V����
                    AIController->Possess(SpawnedBeyblade);
                    UE_LOG(LogTemp, Log, TEXT("Manually spawned and possessed AI Controller for %s"), *SpawnedBeyblade->GetName());
                }
            }
        }
        else
        {
            // --- �]���̃v���C���[�R���g���[���[�̃|�[�b�V�������W�b�N ---
            int possesIndex = (Index == 1) ? 0 : Index - 1;
            APlayerController *PlayerController = UGameplayStatics::GetPlayerController(this, possesIndex);
            if (PlayerController)
            {
                PlayerController->Possess(SpawnedBeyblade);
            }
        }
    }
}

// ���t���[���̏���
void ABeybladeGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!MainCamera || !GameInstance)
        return;

    // ���݂̃J�����ʒu���擾
    FVector CameraLocation = MainCamera->GetActorLocation();

    // �Q�[���C���X�^���X����x�C�u���[�h���X�g���擾
    TMap<int32, ABeybladeBase *> BeybladeList = GameInstance->BeybladeList;

    // �}�E�X�J�[�̕\����Ԃ��X�V
    if (BeybladeGameState == EGameState::Playing)
    {
        UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
    }

    // �x�C�u���[�h���X�g�̗v�f��1�ɂȂ�����i�����҂����܂�����j
    if (BeybladeList.Num() == 1)
    {
        // �Q�[����Ԃ��u�I���v�ɐݒ�
        BeybladeGameState = EGameState::Finished;

        // ���X�g�̍ŏ��́i�B��́j�v�f���擾
        TMap<int32, ABeybladeBase *>::TConstIterator BeybladeListConstIterator = BeybladeList.CreateConstIterator();
        ABeybladeBase *FirstBeyblade = BeybladeListConstIterator.Value();

        // ���҂ւ̃Y�[���A�b�v�J�����ʒu���v�Z
        FVector NewLocation = UKismetMathLibrary::VLerp(CameraLocation, FirstBeyblade->GetActorLocation() + FVector(-15.0f, 0.0f, 30.0f), 0.2f);

        // �J������V�����ʒu�ֈړ�
        MainCamera->SetActorLocation(NewLocation, false, nullptr, ETeleportType::None);

        // ���҃C���f�b�N�X������
        int WinnerIndex = (FirstBeyblade->GetControllerType() == EBeybladeControllerType::P1) ? 1 : 2;

        // �I���E�B�W�F�b�g���쐬���ĕ\��
        if (FinishWidgetClass)
        {
            if (!FinishWidget)
            {
                FinishWidget = CreateWidget<UUserWidget>(GetWorld(), FinishWidgetClass, FName("Finish Widget"));
            }
            // �E�B�W�F�b�g�̏������֐����Ăяo��
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

    // �K�E�Z�̃��C�g�G�t�F�N�g���I���̏ꍇ
    if (EnvironmentManager->GetIsUltraLightOn())
    {
        ABeybladeBase *UltimateUser = GameInstance->GetUltimateUser();
        if (UltimateUser)
        {
            // �K�E�Z�g�p�҂�ǂ��悤�ɃJ�������ړ�
            FVector UltimateUserLocation = UltimateUser->GetActorLocation();
            FVector NewLocation = UKismetMathLibrary::VLerp(CameraLocation, UltimateUserLocation + FVector(-50.0f, 0.0f, 100.0f), 0.05f);

            MainCamera->SetActorLocation(NewLocation, false, nullptr, ETeleportType::None);

            // �K�E�ZUI��\��
            UltraWidget->SetVisibility(ESlateVisibility::HitTestInvisible);

            // �X�|�b�g���C�g��K�E�Z�g�p�҂̏�Ɉړ�
            EnvironmentManager->MoveSpotlight(UltimateUserLocation + FVector(0.0f, 0.0f, 300.0f));
        }
    }
    // �ʏ펞�i�x�C�u���[�h��2�̈ȏ�j�̃J��������
    else if (BeybladeList.Num() > 1)
    {
        // �J���������̈ʒu�Ɋ��炩�ɖ߂�
        FVector NewLocation = UKismetMathLibrary::VLerp(CameraLocation, CameraOriginalPos, 0.05f);
        MainCamera->SetActorLocation(NewLocation, false, nullptr, ETeleportType::None);

        // �K�E�ZUI���\��
        UltraWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (BeybladeGameState == EGameState::Finished)
    {
        GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
        // �Q�[�����I��������}�E�X�J�[�\����\��
    }
}

// ���̓f�o�C�X�̐ڑ���Ԃ��ύX���ꂽ�Ƃ��̏���
void ABeybladeGameModeBase::OnInputDeviceConnectionChange(EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId)
{
    FString InputDeviceIdStr = UKismetStringLibrary::Conv_InputDeviceIdToString(InputDeviceId);
    FString PlatformUserIdStr = UKismetStringLibrary::Conv_PlatformUserIdToString(PlatformUserId);

    // 2P�R���g���[���[�̐ڑ��Ɋ֘A����Ǝv���鏈��
    // ID��"1"�̃f�o�C�X���v���b�g�t�H�[��ID��"1"�̏ꍇ
    bool bIsInputDeviceIdOne = UKismetStringLibrary::EqualEqual_StrStr(InputDeviceIdStr, TEXT("1"));
    bool bIsPlatformUserIdOne = UKismetStringLibrary::EqualEqual_StrStr(PlatformUserIdStr, TEXT("1"));

    if (bIsInputDeviceIdOne && bIsPlatformUserIdOne)
    {
        // �ڑ���Ԃɉ����ăQ�[���p�b�hID�̃I�t�Z�b�g��ݒ�
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

// �|�[�Y��Ԃ̐؂�ւ�
void ABeybladeGameModeBase::TogglePause(int32 PlayerIndexWhoRequestedPause)
{
    // �v���C���O��Ԃ���|�[�Y��Ԃɐ؂�ւ���ꍇ
    if (BeybladeGameState == EGameState::Playing)
    {
        BeybladeGameState = EGameState::Paused;
        PausedPlayerIndex = PlayerIndexWhoRequestedPause; // �|�[�Y�����v���C���[���L�^
        UGameplayStatics::SetGamePaused(GetWorld(), true);
        if (PauseWidget)
        {
            // �}�E�X�\��
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
        /*�|�[�Y���������v���C���[�{�l�̂݉����\*/
        if (PlayerIndexWhoRequestedPause != PausedPlayerIndex && PausedPlayerIndex != -1)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player %d cannot unpause, game was paused by Player %d"), PlayerIndexWhoRequestedPause, PausedPlayerIndex);
            return;
        }

        BeybladeGameState = EGameState::Playing;
        PausedPlayerIndex = -1; // ���Z�b�g
        UGameplayStatics::SetGamePaused(GetWorld(), false);
        if (PauseWidget)
        {
            PauseWidget->SetVisibility(ESlateVisibility::Hidden);
            // �}�E�X�\��
            APlayerController *Player0Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (Player0Controller)
            {
                Player0Controller->SetShowMouseCursor(false);
            }
        }
    }
}