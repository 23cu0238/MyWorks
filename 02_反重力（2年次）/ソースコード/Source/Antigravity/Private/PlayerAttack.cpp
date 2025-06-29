// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAttack.h"
#include "IDeadable.h"

UConventionalAttack::UConventionalAttack()
{
}

UConventionalAttack::UConventionalAttack(AActor *owner)
	: mOwner(owner)
{
}

UConventionalAttack::~UConventionalAttack()
{
}

void UConventionalAttack::Attack(float damage, AActor *target, FVector position)
{
	Attack(damage, target);
}

void UConventionalAttack::Attack(float damage, AActor* target)
{
	if (IDeadable* ab = Cast<IDeadable>(target))
	{
		ab->FluctuationHP(damage);
		// OtherComp->AddImpulseAtLocation(mOwner->GetVelocity() * 100.0f, mOwner->GetActorLocation());
	}
}
