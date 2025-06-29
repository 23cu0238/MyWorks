// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GimmickManager.h"
#include "EventObject.generated.h"


class UMyGameInstance;

UCLASS()
class ANTIGRAVITY_API AEventObject : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AEventObject();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool Overlap(AActor* hitActor);

	UFUNCTION(BlueprintCallable)
	bool OutEvent(AActor* hitActor);
	UFUNCTION(BlueprintCallable)
	inline bool IsBreak()const { return mIsBreak; }

	UFUNCTION(BlueprintCallable)
	inline bool IsPlayEvent()const { return mPlayEvent; }

	UFUNCTION(BlueprintCallable)
	inline void SetEvent() { mPlayEvent = !mPlayEvent; }
private:
	UPROPERTY(EditAnywhere)
	bool mIsLoop;

	bool mOnEvent;

	bool mIsBreak;

	UPROPERTY(EditAnywhere)
	bool mPlayEvent;

	UPROPERTY(EditAnywhere)
	int HP;

	UPROPERTY(EditAnywhere)
	FName ID;

	UPROPERTY(EditAnywhere)
	TEnumAsByte< EventType::Type> InEventType;

	UPROPERTY(EditAnywhere)
	TEnumAsByte< EventType::Type> OutEventType;

	UPROPERTY()
	UMyGameInstance* instance;

};
