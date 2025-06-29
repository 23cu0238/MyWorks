// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeRespone.h"
#include "SavePoint.h"
#include "GameFramework/PlayerController.h"

void AMyGameModeRespone::RespawnPlayer(AController* PlayerController)
{
    if (PlayerController && ASavePoint::RespawnLocation != FVector::ZeroVector)
    {
        APawn* PlayerPawn = PlayerController->GetPawn();
        if (PlayerPawn)
        {
            PlayerPawn->SetActorLocation(ASavePoint::RespawnLocation);
            UE_LOG(LogTemp, Log, TEXT("Player Respawned at: %s"), *ASavePoint::RespawnLocation.ToString());
        }
    }
}


//これより下のプログラムでリスポーン座標を呼び出ししてリスポーンする(直前に保存された座標が呼び出される)
//void SomeFunction(AController* PlayerController)
//{
//    // ゲームモードを取得
//    AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(PlayerController->GetWorld()));
//
//    if (GameMode)
//    {
//        // RespawnPlayerを呼び出す
//        GameMode->RespawnPlayer(PlayerController);
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("GameMode is not of type AMyGameMode!"));
//    }
//}