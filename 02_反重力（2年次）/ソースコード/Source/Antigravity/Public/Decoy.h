// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Decoy.generated.h"

class AEnemyBase;

UCLASS()
class ANTIGRAVITY_API ADecoy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADecoy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	virtual void OnOverlap(AActor*HitActor);
	UPROPERTY()
	float timer;
	UPROPERTY()
	TArray<AEnemyBase*> mHitEnemys;
};
