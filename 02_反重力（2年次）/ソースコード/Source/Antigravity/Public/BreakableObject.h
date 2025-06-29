// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakableObject.generated.h"
class USoundCue;

UCLASS()
class ANTIGRAVITY_API ABreakableObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABreakableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor *HitActor);
	UFUNCTION(BlueprintCallable)
	inline bool IsBreak() const { return mIsBreak; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue *mBreakingSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HP;

	bool mIsBreak;
};
