// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TESTOBJECT.generated.h"

UCLASS()
class ANTIGRAVITY_API ATESTOBJECT : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATESTOBJECT();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void FindAndMoveToLocation(FVector TargetLocation);
private:
	FVector Start;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	FVector End;
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TArray<FVector> anchorPoint;

    /// <summary>
    /// ベジェ曲線関数
    /// </summary>
    FVector Evaluate(float t);

    int cunt = 0;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float speed;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float mRotationSpeed;
    float timer = 0;
};
