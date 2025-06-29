// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"MissileState.h"
#include "Components/ActorComponent.h"
#include "ChargeState.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANTIGRAVITY_API UChargeState : public UActorComponent, public IMissileState
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChargeState();
	void OnEnter(UMissile* owner)override;
	void OnUpdate(UMissile* owner)override;
	void OnExit(UMissile* owner)override;
private:
	UMissile* mOwner;
	float ChargeTimer;
};
