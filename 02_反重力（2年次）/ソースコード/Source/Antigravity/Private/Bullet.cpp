// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerAttack.h"
#include "BulletContainer.h"
#include"MyGameInstance.h"

ABullet::ABullet()
{
	if(mTypeName == "")
	mTypeName = "Default";
	SetUp();
}

// 初期化
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	if (!GetWorld()->GetGameInstance())
	{
		GetWorld()->BeginPlay();
	}
	UMyGameInstance* instance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (instance != nullptr)
		bulletState = instance->GetBulletContainer()->SetupState(mTypeName);
	if (bulletState != nullptr) 
	{
		bulletState->Init();
	}
	else
		UE_LOG(LogTemp, Log, TEXT("BULLETSTATE IS NULL"));
	mDestroyTimer = 0;
}


// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bulletState != nullptr)
	{
		bulletState->Update(DeltaTime);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("BULLETSTATE IS NULL"));
	}

	mDestroyTimer += GetWorld()->GetDeltaSeconds();

	if (mDestroyTimer > 3)
		Destroy();
}

void ABullet::OnHit(/*UPrimitiveComponent *HitComp, */AActor *OtherActor/*, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit*/)
{
	bool b = false;
	if (bulletState != nullptr)
	{
		b = bulletState->Hit(OtherActor);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("BULLETSTATE IS NULL"));
	}
	if(b && !mIsRevers)
	Destroy();
}

void ABullet::SetUp()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	//AActor::AddComponentByClass(USphereComponent);
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	//CollisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = mInitialSpeed;
	ProjectileMovement->MaxSpeed = mMaxSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
}

void ABullet::SetOriginator(AActor* originator)
{
	if (!originator)
		return;
	mOriginator = originator;
}

AActor* ABullet::GetOriginator() const
{
	return mOriginator;
}
