// Fill out your copyright notice in the Description page of Project Settings.

#include "RecoveryMp.h"
#include "PlayerCharacter.h"

ARecoveryMp::ARecoveryMp()
{
	// Meshの設定
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Characters/Object/Gravite_Item/Gravity_Item.Gravity_Item'"));
	if (MeshAsset.Succeeded())
	{
		Mesh2 = MeshAsset.Object;
		SetMesh(Mesh2);
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh2 is not set for RecoveryMp."));
	}
}

void ARecoveryMp::BeginPlay()
{
	Super::BeginPlay();

	GravityInitialize();

	// アイテムの傾き
	TiltItem(mTilt);

	mpBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARecoveryMp::OnOverlapRecovery);
	mpBoxComp->OnComponentEndOverlap.AddDynamic(this, &ARecoveryMp::OnOverlapRecoveryEnd);
	mpBoxComp->SetupAttachment(RootComponent);
	mpBoxComp->SetWorldLocation(GetActorLocation());
}

void ARecoveryMp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 重力付与
	GravityScale(DeltaTime);

	// アイテムの回転
	RotateItem(mItemRotationSpeed);

	// 上下移動
	UpdownAmplitude();

	mpBoxComp->SetWorldLocation(GetActorLocation());
}

void ARecoveryMp::OnOverlapRecovery(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Item is Hit"));

	// 当たった物がPlayerか確認
	APlayerCharacter *PlayerActor = Cast<APlayerCharacter>(OtherActor);
	if (PlayerActor == nullptr)
	{
		return;
	}
	else
	{
		isHit = true;

		// 上限重力ゲージを確認
		if (PlayerActor->GetMaxGravityGauge() <= 300.0f)
		{
			// Playerの重力変更ゲージの上限を上げる
			PlayerActor->IncreaseMaxGravityGauge(mItemAddMP);
		}
	}
}

void ARecoveryMp::OnOverlapRecoveryEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
}
