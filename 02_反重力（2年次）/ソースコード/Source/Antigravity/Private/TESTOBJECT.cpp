// Fill out your copyright notice in the Description page of Project Settings.


#include "TESTOBJECT.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ATESTOBJECT::ATESTOBJECT()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATESTOBJECT::BeginPlay()
{
	Super::BeginPlay();
	Start = GetActorLocation();
}

// Called every frame
void ATESTOBJECT::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector v = Evaluate(timer);
	timer = UGameplayStatics::GetTimeSeconds(GetWorld()) * speed;
	FindAndMoveToLocation(v);
	SetActorLocation(v);
}

// Called to bind functionality to input
void ATESTOBJECT::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/// <summary>
/// ベジェ曲線関数
/// </summary>
FVector ATESTOBJECT::Evaluate(float t)
{
	if (anchorPoint.Num() == 0)
	{
		return FVector(0, 0, 0);
	}

	FVector result;
	
	if (cunt > anchorPoint.Num())
	{
		cunt = 0;
	}
	result = (FMath::Pow((1 - t), 3) * Start) + 3 * (1 - t) * (FMath::Pow(t, 2) * anchorPoint[cunt]) + (FMath::Pow(t, 3) * End);
	
	return result;
}

void ATESTOBJECT::FindAndMoveToLocation(FVector TargetLocation)
{
	// 現在の位置を取得
	FVector CurrentLocation = GetActorLocation();

	// ターゲット位置までの距離を計算
	float Distance = FVector::Dist(TargetLocation, CurrentLocation);


	// ターゲット方向を向くように回転（Yaw のみ変更）
	FRotator CurrentRotation = GetActorRotation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), mRotationSpeed);
	SetActorRotation(FRotator(NewRotation.Pitch, CurrentRotation.Yaw, CurrentRotation.Roll)); // Yaw のみ更新
}