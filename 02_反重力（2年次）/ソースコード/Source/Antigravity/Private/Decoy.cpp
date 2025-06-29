// Fill out your copyright notice in the Description page of Project Settings.


#include "Decoy.h"
#include"EnemyBase.h"

// Sets default values
ADecoy::ADecoy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADecoy::BeginPlay()
{
	Super::BeginPlay();
	timer = 0;
}

// Called every frame
void ADecoy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mHitEnemys.Num() > 0)
	{
		for (int i = 0; i < mHitEnemys.Num(); ++i)
		{
			if (IsValid(mHitEnemys[i]))
			{
				mHitEnemys[i]->FindAndMoveToLocation(GetActorLocation());
			}
		}
	}

	timer += GetWorld()->GetDeltaSeconds();

	if (timer > 10)
		Destroy();
}

void ADecoy::OnOverlap(AActor* HitActor)
{
	if (!HitActor)
		return;
	AEnemyBase* p = Cast<AEnemyBase>(HitActor);
	if (!p)
		return;

	if (p->GetStatus() == "Chase")
		return;
	
	mHitEnemys.Add(p);
}

