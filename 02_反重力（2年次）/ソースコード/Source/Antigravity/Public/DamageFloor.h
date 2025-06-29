// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageFloor.generated.h"


class ConventionalAttack;
UCLASS()
class ANTIGRAVITY_API ADamageFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageFloor();
	/** called when projectile hits something */
	UFUNCTION(BlueprintCallable)
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	float mTimer = 0;
	float mExtinguishTime;
	TObjectPtr<UConventionalAttack> mAttack;
	float mDamage;

	FVector HitNormal;
};
