// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UAnimSequence;

UENUM(BlueprintType)
	enum class State : uint8
	{
		close = 0,
		open = 1,
	};


UCLASS()
class ANTIGRAVITY_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ChanegState(int num);
	bool DoorEvent();
	FName GetTag()const;
	UFUNCTION(BlueprintCallable)
	State GetState() const { return mState; }

private:
	void Init();
	bool IsReached(FVector, FVector);
private:
	UPROPERTY(EditAnywhere)
	float mSpeed;
	UPROPERTY(EditAnywhere)
	FName ID;
	UPROPERTY(EditAnywhere)
	FVector mOpenPosition;
	UPROPERTY(EditAnywhere)
	FVector mClosePosition;
	UPROPERTY(EditAnywhere)
	State mState;
	bool IsInited = false;
};
