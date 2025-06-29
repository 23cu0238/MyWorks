// Fill out your copyright notice in the Description page of Project Settings
//<<<<<<< HEAD
#include "IDeadable.h"
//=======
#include "ItemInteractable.h"
//>>>>>>> f406533f227dcf8ab0fba6555e2bef168b536ab2
#pragma once
#include "ObjectBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VendingMachine.generated.h"

UCLASS()
class ANTIGRAVITY_API AVendingMachine : public AActor, public ObjectBase, public IDeadable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVendingMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// アイテムを出すかを判断する関数
	// bool discharge();

private:
	// アイテムをスポーンする関数
	void SpawnActors();

private:
	float mHealth; // HPを管理する変数

public:
	// 自動販売機メッシュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent *mVendingMachineMesh;

	//// ItemInteractableの実装
	// virtual void OnPlayerInteract(AActor* Player) override;

private:
	FVector Velocity;

	float GravityForce;

	// IDeadableインターフェースの関数をオーバーライド
public:
	virtual void FluctuationHP(float Decrease) override;
	virtual bool Dead() override;
};