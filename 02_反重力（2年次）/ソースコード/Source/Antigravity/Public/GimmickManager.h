// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GimmickManager.generated.h"

class ADoor;
class AEventArea;
class UMyGameInstance;

UENUM()
namespace EventType {
	enum Type
	{
		None = 0,
		Closed = 1,
		Open = 2,	
		OnNextEvent = 3,
		Save = 4,
		End = 100,
	};
}

USTRUCT()
struct FEventData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	TEnumAsByte< EventType::Type> Eventtype;
	UPROPERTY(EditAnywhere)
	FName NextID;
	UPROPERTY(EditAnywhere)
	FVector Location;
};
UCLASS()
class ANTIGRAVITY_API AEventManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEventManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetDoor(ADoor* door);
	void SetmEvents(AEventArea* area);
	void SetNextLavelName(FName);
	void Release(FName,EventType::Type);
	void Release(FName,FEventData);

	UFUNCTION(BlueprintCallable)
	inline bool IsEnd()const { return mIsEnd; };
	UFUNCTION(BlueprintCallable)
	inline bool IsPlaySave()const { return mIsPlaySaveAnim; }
	UFUNCTION(BlueprintCallable)
	inline void OffSaveAnim() { mIsPlaySaveAnim = false; }
private:
	TArray<ADoor*> mDoors;
	TArray<AEventArea*>mEventAreas;
	UPROPERTY(EditAnywhere)
	FName LevelToLoad;
	bool mIsEnd;
	bool mIsPlaySaveAnim;
};
