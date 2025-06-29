// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IAttack.generated.h"

class IDeadable;
/**
 * 
 */

UINTERFACE(MinimalAPI)
class  UAttackable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ANTIGRAVITY_API IAttackable
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual void Attack(float damage, AActor* target, FVector position) = 0;
};
