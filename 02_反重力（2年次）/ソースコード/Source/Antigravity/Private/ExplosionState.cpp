// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionState.h"
#include "Missile.h"
// Sets default values for this component's properties
UExplosionState::UExplosionState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UExplosionState::OnEnter(UMissile* owner)
{
	//構造体作成
	FActorSpawnParameters ActorSpawnParams;
	//生成ルール指定
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//念のため、ログを出しておきます
	UE_LOG(LogTemp, Log, TEXT("ShootingAttack"));
	//生成
	//ABullet* a = pWorld->SpawnActor<ABullet>(BulletClass, Start, direction, ActorSpawnParams);

	GetOwner()->Destroy();
}
void UExplosionState::OnUpdate(UMissile* owner)
{

}
void UExplosionState::OnExit(UMissile* owner)
{

}