// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableObject.h"
#include"PlayerCharacter.h"

// Sets default values
ABreakableObject::ABreakableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABreakableObject::BeginPlay()
{
	Super::BeginPlay();
	mIsBreak = false;
}

// Called every frame
void ABreakableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableObject::OnOverlap(AActor* HitActor)
{
	APlayerCharacter* p = Cast<APlayerCharacter>(HitActor);
	if (!p)
		return;
		
	if (p->GetPowerLevel() < HP)
		return;

	mIsBreak = true;
}


