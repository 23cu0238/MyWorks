// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeRespone.generated.h"

/**
 * 
 */
UCLASS()
class ANTIGRAVITY_API AMyGameModeRespone : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void RespawnPlayer(AController* PlayerController);
};