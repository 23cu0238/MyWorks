// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Shootable.h"
#include "DefaultBullet.generated.h"

class AActor;
class ABullet;
class UConventionalAttack;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANTIGRAVITY_API UDefaultBullet : public UActorComponent , public IShootable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDefaultBullet();

public:	
	void Init();
	void Update(float DeltaTime);
	bool Hit(AActor* OtherActor);
	void HitStop(float DeltaTime);
private:
	FVector Start;
	UPROPERTY()
	ABullet* mBullet;
	UConventionalAttack* mAttack;
	float mHitstopTimer = 0.0f;
};
