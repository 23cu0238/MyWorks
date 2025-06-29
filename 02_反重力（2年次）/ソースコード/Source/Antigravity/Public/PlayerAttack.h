// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IAttack.h"
#include "Components/ActorComponent.h"
#include "PlayerAttack.generated.h"

/**
 * 
 */
UCLASS()
class ANTIGRAVITY_API UConventionalAttack : public UObject, public IAttackable
{
	GENERATED_BODY()
public:
	UConventionalAttack();
	UConventionalAttack(AActor* owner);
	~UConventionalAttack();
public:
	void Attack(float damage, AActor* target,FVector position)override;
	void Attack(float damage, AActor* target);
private:
	AActor* mOwner;
};
