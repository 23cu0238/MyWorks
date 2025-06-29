// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"

#include "Bullet.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class ConventionalAttack;
class IShootable;

UCLASS()
class ANTIGRAVITY_API ABullet : public AActor
{
	GENERATED_BODY()

protected:
	// èâä˙âª
	virtual void BeginPlay() override;

public:
	ABullet();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** called when projectile hits something */
	UFUNCTION(BlueprintCallable)
	virtual void OnHit(/*UPrimitiveComponent* HitComp, */ AActor *OtherActor /*, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit*/);
	void SetUp();
	void SetOriginator(AActor *);
	AActor *GetOriginator() const;

public:
	IShootable *bulletState;

private:
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	TObjectPtr<USphereComponent> CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	/** Sphere collision component */
	UPROPERTY(EditAnywhere)
	bool mIsRevers;

	/** Sphere collision component */
	UPROPERTY(EditAnywhere)
	FString mTypeName;
	float mDestroyTimer;
	AActor *mOriginator;

	UPROPERTY(EditAnywhere)
	float mInitialSpeed;

	UPROPERTY(EditAnywhere)
	float mMaxSpeed;
};
