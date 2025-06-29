// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include"IAttack.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrossAttack.generated.h"

UCLASS()
class ANTIGRAVITY_API ACrossAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrossAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	IAttackable* mAttack;
};
