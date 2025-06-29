// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GimmickManager.h"
#include "MonsterHouse.generated.h"

class UMyGameInstance;



UCLASS()
class ANTIGRAVITY_API AEventArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEventArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	bool Overlap(AActor* hitActor);
	UFUNCTION(BlueprintCallable)
	bool OverlapEnd(AActor* hitActor);
	UFUNCTION(BlueprintCallable)
	void  Subtract();
	UFUNCTION(BlueprintCallable)
	inline FName GetID()const { return ID; };
	UFUNCTION(BlueprintCallable)
	inline bool IsStart()const { return mStart; }
	UFUNCTION(BlueprintCallable)
	inline AActor* GetTarget(int i) { return mTargetList[i]; }
	UFUNCTION(BlueprintCallable)
	inline int GetTargetArrey()const { return mTargetList.Num(); }
	UFUNCTION(BlueprintCallable)
	inline bool IsLoop()const { return mIsLoop; }
	UFUNCTION(BlueprintCallable)
	inline bool mIsEvent()const { return mOnPlayEvent; }
	UFUNCTION(BlueprintCallable)
	inline void ChangeEvent() { mOnPlayEvent = !mOnPlayEvent; }

private:
	void Init();

private:
	UPROPERTY(EditAnywhere)
	int mAliveCount;

	bool mIsClear;
	bool mStart;
	bool IsInited;

	UPROPERTY(EditAnywhere)
	bool mIsLoop;
	UPROPERTY(EditAnywhere)
	bool mOnPlayEvent;
	UPROPERTY(EditAnywhere)
	FName ID;
	UPROPERTY(EditAnywhere)
	TArray<FEventData> mInEventData;
	UPROPERTY(EditAnywhere)
	TArray<FEventData> mOutEventData;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> mTargetList;
	UPROPERTY()
	UMyGameInstance* instance;
};
