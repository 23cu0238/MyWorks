// Fill out your copyright notice in the Description page of Project Settings.
#include "MyGameInstance.h"
#include "BulletContainer.h"
#include "GimmickManager.h"
#include "Engine/Engine.h"

UMyGameInstance *UMyGameInstance::instance = nullptr;

UMyGameInstance *UMyGameInstance::GetInstance()
{
	if (GEngine && instance == nullptr)
	{
		FWorldContext *context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		instance = Cast<UMyGameInstance>(context->OwningGameInstance);
	}

	return instance;
}

ABulletContainer *UMyGameInstance::GetBulletContainer()
{
	if (BulletContainer == nullptr)
	{
		GetWorld()->BeginPlay();
	}

	return BulletContainer;
}

AEventManager *UMyGameInstance::GetEventManager()
{
	if (!EventManager)
		GetWorld()->BeginPlay();

	return EventManager;
}

void UMyGameInstance::GenerateBulletContainer(TSubclassOf<ABulletContainer> bulletContainer)
{
	BulletContainer = GetWorld()->SpawnActor<ABulletContainer>(bulletContainer);
}

void UMyGameInstance::GenerateGimmickManager(TSubclassOf<AEventManager> gimmickManager)
{
	EventManager = GetWorld()->SpawnActor<AEventManager>(gimmickManager);
}
