// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDeadable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class  UDeadable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ANTIGRAVITY_API IDeadable
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual bool Dead();
	virtual void FluctuationHP(float decrease);

};