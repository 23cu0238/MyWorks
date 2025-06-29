// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageFloor.h"
#include"PlayerAttack.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ADamageFloor::ADamageFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADamageFloor::BeginPlay()
{
	Super::BeginPlay();
	mAttack = NewObject<UConventionalAttack>();
	mTimer = mExtinguishTime;
}

// Called every frame
void ADamageFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	mTimer -= UGameplayStatics::GetTimeSeconds(GetWorld());

	if (mTimer < 0)
		Destroy();
}

void ADamageFloor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Player"))
		mAttack->Attack(mDamage, OtherActor);
}
