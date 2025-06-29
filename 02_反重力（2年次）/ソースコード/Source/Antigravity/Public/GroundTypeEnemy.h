// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.h"
#include "GroundTypeEnemy.generated.h"

class UShootingAttack;
class USphereComponent;

UCLASS()
class ANTIGRAVITY_API AGroundTypeEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGroundTypeEnemy();

	// デストラクタ
	~AGroundTypeEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void UpdateMove(float DeltaTime)override;

private:
	UFUNCTION(BlueprintCallable)
	void OnOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnOverlapAttackEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void AttackFunction(AActor* OtherActor, UPrimitiveComponent* OtherComp);
	UFUNCTION(BlueprintCallable)
	bool PlayAttack();
private:
	// 射撃クラス
	UPROPERTY(EditAnywhere, Category = "EnemyAttack")
	UShootingAttack* mShootiongAttack;

	UPROPERTY(EditAnywhere, Category = "EnemyAttack")
	FString mShootingName = "Normal";
	
	UPROPERTY(EditAnywhere, Category = "EnemyAttack")
	float mShootingRadius;

	UPROPERTY(EditAnywhere, Category = "EnemyAttack")
	float mShootingCoolTime;

	UPROPERTY(EditAnywhere, Category = "EnemyAttack")
	float mShootingTime;

	TObjectPtr<USphereComponent> mShootingRange;
};
