// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Kismet/KismetMathLibrary.h"
#include"MyGameInstance.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	if (mSpeed < 0)
		mSpeed = 1;
	IsInited = false;
	Init();
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Init();
	DoorEvent();
}

void ADoor::Init()
{
	if (IsInited)
		return;
	UMyGameInstance* instance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (instance == nullptr)
		return;
	AEventManager* a = instance->GetEventManager();
	if (a == nullptr)
		return;
	a->SetDoor(this);
	IsInited = true;
}

bool ADoor::IsReached(FVector v1, FVector v2)
{
	if ((v1 - v2).Length() > 2.0f) {
		return false;
	}

	return true;
}

void ADoor::ChanegState(int num)
{
	//KOKODEANIMATION
	UE_LOG(LogTemp, Log, TEXT("Open!"));
	switch (num)
	{
	case(0):
		mState = State::close;
		break;
	case(1):
		mState = State::open;
		break;
	default:
		return;
	}
}

bool ADoor::DoorEvent()
{
	if (mState == State::close)
	{
		if(IsReached(GetActorLocation(), mClosePosition))
			return false;

		SetActorLocation(UKismetMathLibrary::VLerp(GetActorLocation(), mClosePosition, mSpeed));
	}
	else 
	{
		if (IsReached(GetActorLocation(), mOpenPosition))
			return false;

		SetActorLocation(UKismetMathLibrary::VLerp(GetActorLocation(), mOpenPosition, mSpeed));
	}

	return true;
}


FName ADoor::GetTag()const
{
	return ID;
}


