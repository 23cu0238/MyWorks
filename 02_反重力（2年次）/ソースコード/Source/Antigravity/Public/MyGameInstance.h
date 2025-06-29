// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BulletContainer.h"
#include "GimmickManager.h"
#include "MyGameInstance.generated.h"

/**
 *
 */
UCLASS()
class ANTIGRAVITY_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

	static UMyGameInstance *instance;

public:
	static UMyGameInstance *GetInstance();
	ABulletContainer *GetBulletContainer();
	AEventManager *GetEventManager();

	UFUNCTION(BlueprintCallable, Category = CPP_And_Blueprint)
	void GenerateBulletContainer(TSubclassOf<ABulletContainer> bulletContainer);
	UFUNCTION(BlueprintCallable, Category = CPP_And_Blueprint)
	void GenerateGimmickManager(TSubclassOf<AEventManager> gimmickManager);

	UFUNCTION(BlueprintCallable, Category = Time)
	inline float GetDeltaRealtimeSeconds() const { return GetWorld()->GetTime().GetDeltaRealTimeSeconds(); }

	UFUNCTION(BlueprintCallable, Category = CPP_And_Blueprint)
	inline void SetRespawnPosition(FVector nextPos) { mRespawnPosition = nextPos; }

	UFUNCTION(BlueprintCallable, Category = CPP_And_Blueprint)
	inline FVector GetRespawnPosition() const { return mRespawnPosition; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UObject>> mThrowableList;

private:
	UPROPERTY(EditAnywhere)
	ABulletContainer *BulletContainer;
	UPROPERTY(EditAnywhere)
	AEventManager *EventManager;
	UPROPERTY(EditAnywhere)
	FVector mRespawnPosition;
};
