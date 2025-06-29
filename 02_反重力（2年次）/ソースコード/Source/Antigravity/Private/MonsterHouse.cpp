// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHouse.h"
#include "MyGameInstance.h"

// Sets default values
AEventArea::AEventArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsInited = false;
}

// Called when the game starts or when spawned
void AEventArea::BeginPlay()
{
	Super::BeginPlay();
	instance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	mIsClear = false;
	mStart = false;
}

// Called every frame
void AEventArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (mIsClear)
		return;

	Init();

	if (mAliveCount > 0)
		return;
	for (int i = 0; i < mOutEventData.Num(); ++i)
	instance->GetEventManager()->Release(ID,mOutEventData[i]);
	mIsClear = true;
}

bool AEventArea::Overlap(AActor* hitActor)
{
	if (!mOnPlayEvent)
		return false;

	if (!mIsLoop && mStart)
		return false;
	if (!instance)
		return false;

	if (!hitActor)
		return false;

	if (!hitActor->ActorHasTag("Player"))
		return false;

	UE_LOG(LogTemp, Log, TEXT("Trap!"));

	for(int i = 0; i < mInEventData.Num();++i)
	instance->GetEventManager()->Release(ID,mInEventData[i]);

	mStart = true;
	return true;
}

bool AEventArea::OverlapEnd(AActor* hitActor)
{
	if (!mOnPlayEvent)
		return false;

	if (!mIsLoop && mStart)
		return false;

	if (!hitActor)
		return false;

	if (!hitActor->ActorHasTag("Player"))
		return false;

	UE_LOG(LogTemp, Log, TEXT("Trap!"));

	for (int i = 0; i < mOutEventData.Num(); ++i)
		instance->GetEventManager()->Release(ID, mOutEventData[i]);

	mStart = false;
	return true;
}

void AEventArea::Subtract()
{
	UE_LOG(LogTemp, Log, TEXT("Subtracted"));
	--mAliveCount;
}

void AEventArea::Init()
{
	if (IsInited)
		return;
	if (!instance)
		return;
	AEventManager* a = instance->GetEventManager();
	if (a == nullptr)
		return;
	a->SetmEvents(this);
	IsInited = true;
}