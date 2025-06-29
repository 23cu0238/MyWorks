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

	// �_���[�W��^���鏈��
	void ApplyFallDamage();

	void StartFalling();

private:
	// �����������Ԃ��v������ϐ�
	float FallTime;

	// �_���[�W��^���鎞�Ԃ�臒l
	float FallThreshold;

	// �_���[�W�̒l
	float DamageAmount;

	// �I�u�W�F�N�g���������Ă��邩�ǂ����̃t���O
	bool bIsFalling;

};
