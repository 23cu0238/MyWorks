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


//�����艺�̃v���O�����Ń��X�|�[�����W���Ăяo�����ă��X�|�[������(���O�ɕۑ����ꂽ���W���Ăяo�����)
//void SomeFunction(AController* PlayerController)
//{
//    // �Q�[�����[�h���擾
//    AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(PlayerController->GetWorld()));
//
//    if (GameMode)
//    {
//        // RespawnPlayer���Ăяo��
//        GameMode->RespawnPlayer(PlayerController);
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("GameMode is not of type AMyGameMode!"));
//    }
//}