// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Shootable.h"
#include "Missile.generated.h"

class ConventionalAttack;
class IMissileState;
class UInductionState;
class UChargeState;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ANTIGRAVITY_API UMissile : public UActorComponent, public IShootable
{
    GENERATED_BODY()
public:
    UInductionState* inducnt;
    UChargeState* charge;
public:
    // Sets default values for this component's properties
    UMissile();
    void Init()override;
    void Update(float DeltaTime) override;
    ABullet* GetOwnerBullet()const;
    TArray<FVector> GetAnchorPoint()const;
    void ChangeState(IMissileState* nextState);
private:
    void InitState();
private:
    ABullet* mBullet;
    IMissileState* currentState;
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TArray<FVector> anchorPoint;
public:
    FVector HitNormal;
    FRotator SpawnRotation;
};
