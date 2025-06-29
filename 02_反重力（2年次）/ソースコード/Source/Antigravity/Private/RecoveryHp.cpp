// Fill out your copyright notice in the Description page of Project Settings.

#include "RecoveryHp.h"
#include "PlayerCharacter.h"
#include "PlayerAttack.h"

ARecoveryHp::ARecoveryHp()
{
}

void ARecoveryHp::BeginPlay()
{
	Super::BeginPlay();

	GravityInitialize();

	// ?��A?��C?��e?��?��?��̌X?��?��
	TiltItem(mTilt);

	mpBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARecoveryHp::OnOverlapRecovery);
	mpBoxComp->OnComponentEndOverlap.AddDynamic(this, &ARecoveryHp::OnOverlapRecoveryEnd);
	mpBoxComp->SetupAttachment(RootComponent);
	mpBoxComp->SetWorldLocation(GetActorLocation());
}

void ARecoveryHp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ?��d?��͕t?��^
	GravityScale(DeltaTime);

	// ?��A?��C?��e?��?��?��̉�]
	RotateItem(mItemRotationSpeed);

	// ?��㉺?��ړ�
	UpdownAmplitude();

	mpBoxComp->SetWorldLocation(GetActorLocation());
}

// BoxComponent?��ƐڐG?��?��?��Ă΂�?��֐�
void ARecoveryHp::OnOverlapRecovery(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// ?��ڐG?��?��?��?��?��A?��N?��^?��?��m?��F
	APlayerCharacter *PlayerActor = Cast<APlayerCharacter>(OtherActor);
	if (PlayerActor == nullptr)
	{
		return;
	}
	else
	{
		isHit = true;

		float Recovery = -mItemAddHP;
		// ?��̗͂�?��ő�l?��ł�?��?��Ή񕜂�?��Ȃ�
		if (PlayerActor->GetHP() >= 3)	Recovery = 0;

		// HP?��?��
		TObjectPtr<UConventionalAttack> AttackObj = NewObject<UConventionalAttack>();
		AttackObj->Attack(Recovery, OtherActor);
	}
}

// BoxComponent?��Ɣ�ڐG?��?��?��Ă΂�?��֐�
void ARecoveryHp::OnOverlapRecoveryEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
}
