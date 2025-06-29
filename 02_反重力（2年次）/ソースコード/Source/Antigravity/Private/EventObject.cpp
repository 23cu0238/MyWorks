// Fill out your copyright notice in the Description page of Project Settings.


#include "EventObject.h"
#include "MyGameInstance.h"
#include"PlayerCharacter.h"

// Sets default values
AEventObject::AEventObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
// Called when the game starts or when spawned
void AEventObject::BeginPlay()
{
	Super::BeginPlay();
	instance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	mOnEvent = false;
}

// Called every frame
void AEventObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AEventObject::Overlap(AActor* hitActor)
{
	if (!mPlayEvent)
		return false;

	if (mOnEvent || !hitActor)
		return false;

	if (!hitActor->ActorHasTag("Player"))
		return false;

	APlayerCharacter* p = Cast<APlayerCharacter>(hitActor);
	if (!p)
		return false;

	if (p->GetPowerLevel() < HP)
		return false;

	UE_LOG(LogTemp, Log, TEXT("Trap!"));
	instance->GetEventManager()->Release(ID, InEventType);

	mIsBreak = true;
	mOnEvent = true;

	return true;
}

bool AEventObject::OutEvent(AActor* hitActor)
{
	if (!mIsLoop)
		return false;

	if (!mPlayEvent)
		return false;

	if (!mOnEvent)
		return false;

	if (!hitActor)
		return false;

	instance->GetEventManager()->Release(ID, OutEventType);
	mOnEvent = false;

	return true;
}
