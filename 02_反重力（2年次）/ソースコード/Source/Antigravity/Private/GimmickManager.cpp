// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickManager.h"
#include "Door.h"
#include"MonsterHouse.h"
#include "Kismet/GameplayStatics.h"
#include"MyGameInstance.h"
// Sets default values
AEventManager::AEventManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEventManager::BeginPlay()
{
	Super::BeginPlay();
	mIsEnd = false;
	
}

// Called every frame
void AEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!mIsEnd)
		return;

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), FMath::FInterpTo(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), 0.00001f, GetWorld()->GetTime().GetDeltaRealTimeSeconds(), 100.0f));
}

void AEventManager::SetDoor(ADoor* door)
{
	if (!door)
		return;

	mDoors.Add(door);
}

void AEventManager::SetmEvents(AEventArea* area)
{
	if (!area)
		return;

	mEventAreas.Add(area);
}

void AEventManager::SetNextLavelName(FName nextName)
{
	LevelToLoad = nextName;
}

void AEventManager::Release(FName ID, EventType::Type type)
{
	switch (type)
	{
	case EventType::None:
		break;

	case EventType::Closed:
		for (int i = 0; i < mDoors.Num(); ++i)
		{
			if (!mDoors[i])
				continue;
			if (mDoors[i]->GetTag() != ID)
				continue;

			mDoors[i]->ChanegState(EventType::Closed);
		}

		break;
	case EventType::Open:
		for (int i = 0; i < mDoors.Num(); ++i)
		{
			if (!mDoors[i])
				continue;
			if (mDoors[i]->GetTag() != ID)
				continue;

			mDoors[i]->ChanegState(EventType::Open);
		}
		break;
	case EventType::OnNextEvent:
		for (int i = 0; i < mEventAreas.Num(); ++i)
		{
			if (!mEventAreas[i])
				continue;
		}
		break;
	case EventType::Save:	
		break;
	case EventType::End:
		FLatentActionInfo LatentInfo;
		if (LevelToLoad == "")
			return;
		mIsEnd = true;
		break;
	}
}

void AEventManager::Release(FName ID,FEventData data)
{
	switch (data.Eventtype)
	{
	case EventType::Closed:
		for (int i = 0; i < mDoors.Num(); ++i)
		{
			if (!mDoors[i])
				continue;
			if (mDoors[i]->GetTag() != ID)
				continue;

			mDoors[i]->ChanegState(0);
		}

		break;
	case EventType::Open:
		for (int i = 0; i < mDoors.Num(); ++i)
		{
			if (!mDoors[i])
				continue;
			if (mDoors[i]->GetTag() != ID)
				continue;

			mDoors[i]->ChanegState(1);
		}
		break;
	case EventType::OnNextEvent:
		for (int i = 0; i < mEventAreas.Num(); ++i)
		{
			if (!mEventAreas[i])
				continue;
			if (mEventAreas[i]->GetID() != data.NextID)
				continue;

			mEventAreas[i]->ChangeEvent();
		}
		break;
	case EventType::Save:
	{
		UMyGameInstance* instance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
		if (!instance)
			return;
		instance->GetInstance()->SetRespawnPosition(data.Location);
		mIsPlaySaveAnim = true;
	}
		break;
	case EventType::End:
	{
		FLatentActionInfo LatentInfo;
		if (LevelToLoad == "")
			return;
		mIsEnd = true;
	}
		break;
	}
}

