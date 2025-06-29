// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultBullet.h"
#include "PlayerAttack.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Bullet.h"
#include "PlayerCharacter.h"

// Sets default values for this component's properties
UDefaultBullet::UDefaultBullet()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UDefaultBullet::Init()
{
	Start = GetOwner()->GetActorLocation();
	mAttack = NewObject<UConventionalAttack>();
	UE_LOG(LogTemp, Error, TEXT("BULLET OF INITALIZATION"));
}

void UDefaultBullet::Update(float DeltaTime)
{
	HitStop(DeltaTime);
}
bool UDefaultBullet::Hit(AActor* OtherActor)
{
	UE_LOG(LogTemp, Log, TEXT("BULLET IS HIT"));
	if (!OtherActor)
		return false;

	// Only add impulse and destroy projectile if we hit a physics
	if (!OtherActor->ActorHasTag("Player"))
		return false;
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), FMath::FInterpTo(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), 0.01f, GetWorld()->GetTime().GetDeltaRealTimeSeconds(), 10.0f));
	mHitstopTimer = 0.05f;
	mAttack->Attack(1.0f, OtherActor);
	UE_LOG(LogTemp, Log, TEXT("BULLET IS HIT PLAYER"));
	return true;

}

void UDefaultBullet::HitStop(float DeltaTime)
{
	if (mHitstopTimer == 0)
		return;

	mHitstopTimer -= DeltaTime;

	if (mHitstopTimer <= 0)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), FMath::FInterpTo(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), 1.0f, GetWorld()->GetTime().GetDeltaRealTimeSeconds(), 10.0f));
		mHitstopTimer = 0;
	}
}