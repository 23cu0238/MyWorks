// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallDamageObject.generated.h"

UCLASS()
class ANTIGRAVITY_API AFallDamageObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFallDamageObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ダメージを与える処理
	void ApplyFallDamage();

	void StartFalling();

private:
	// 落下した時間を計測する変数
	float FallTime;

	// ダメージを与える時間の閾値
	float FallThreshold;

	// ダメージの値
	float DamageAmount;

	// オブジェクトが落下しているかどうかのフラグ
	bool bIsFalling;

};
