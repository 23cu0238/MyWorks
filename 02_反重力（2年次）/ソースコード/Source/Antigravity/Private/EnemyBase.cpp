// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.h"
#include "PlayerAttack.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "MonsterHouse.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Sets default values
AEnemyBase::AEnemyBase()
	: mMoveSpeed(0.0f)
	, mAlertComp(nullptr)
	, mAlertRadius(0.0f)
	, mCStatus(ChaseStatus::Idle)
	, mPlayerPos(FVector::ZeroVector)
	, mchaseStopDistance(0.0f)
	, mMovementinterval(0.0f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 警戒範囲設定
	USphereComponent *AlertComp = CreateDefaultSubobject<USphereComponent>(TEXT("AlertRange"));
	AlertComp->SetupAttachment(RootComponent);
	AlertComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnActorBeginOverlap);
	mAlertComp = AlertComp;

	// 索敵範囲設定
	UBoxComponent* SearchComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SearchRange"));
	SearchComp->SetupAttachment(RootComponent);
	SearchComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnOverlapSearch);
	SearchComp->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::OnOverlapSearchEnd);
	mSearchComp = SearchComp;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	// 警戒�?囲の設�?
	mAlertComp->SetActive(false);
	mAlertComp->SetSphereRadius(mAlertRadius);

	mRandomrotateinfo.ElapsedTime = 0.0f; // 経過時間
	mRandomrotateinfo.DeltaTime = GetWorld()->GetDeltaSeconds();

	mRandomMoveInfo.MinDistance = 500.0f;
	mRandomMoveInfo.MoveRange = 1500.0f;
	mRandomMoveInfo.RandomPos = GetActorLocation();

	// MonsterHouse登録
	TArray<AActor *> MonsterHouses;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEventArea::StaticClass(), MonsterHouses);

	for (AActor *Actor : MonsterHouses)
	{
		AEventArea *MonsterHouse = Cast<AEventArea>(Actor);
		if (MonsterHouse)
		{
			// ID検索
			if (MonsterHouse->GetID() == GetMonsterHouseID())
			{
				mMonsterHouse = MonsterHouse;
				// mMonsterHouse->SetTarget(*this);
			}
		}
	}
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool AEnemyBase::Dead()
{
	// UE_LOG(LogTemp, Log, TEXT("てき�?�つたえたい想�?"));
	if (HP <= 0)
	{
		if (mMonsterHouse)
		{
			UE_LOG(LogTemp, Log, TEXT("Subtract"));
			mMonsterHouse->Subtract();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("MH invalid."));
		}
		return true;
	}

	return false;
}
void AEnemyBase::FluctuationHP(float decrease)
{
	UE_LOG(LogTemp, Log, TEXT("Enemy HP decreased"));
	HP = HP - decrease;
}

void AEnemyBase::UpdateMove(float DeltaTime)
{
}

void AEnemyBase::RandomMove()
{
	if (!GetWorld()) return;

	if (!mRandomMoveInfo.IsEndMove)
	{
		mRandomMoveInfo.IsEndMove = true;
		mRandomMoveInfo.ElapsedTime = 0.0f;
	}

	mRandomMoveInfo.ElapsedTime += GetWorld()->GetDeltaSeconds();
	UE_LOG(LogTemp, Log, TEXT("Waiting... ElapsedTime: %f"), mRandomMoveInfo.ElapsedTime);

	if (mRandomMoveInfo.ElapsedTime < 5.0f) return;

	// 5秒経過後、新しい目標地点を設定
	FVector NewTarget;
	do
	{
		float RandX = FMath::FRandRange(-mRandomMoveInfo.MoveRange, mRandomMoveInfo.MoveRange);
		float RandY = FMath::FRandRange(-mRandomMoveInfo.MoveRange, mRandomMoveInfo.MoveRange);
		NewTarget = mInitPos + FVector(RandX, RandY, 0.0f);
	} while (FVector::Dist(NewTarget, mRandomMoveInfo.PreviousPos) < mRandomMoveInfo.MinDistance);

	mRandomMoveInfo.PreviousPos = mRandomMoveInfo.RandomPos;
	mRandomMoveInfo.RandomPos = NewTarget;

	UE_LOG(LogTemp, Log, TEXT("New Target Position: %s"), *mRandomMoveInfo.RandomPos.ToString());

	mRandomMoveInfo.IsEndMove = false;
	mRandomMoveInfo.ElapsedTime = 0.0f;

	// 目標地点が遠いときのみ移動
	if (FVector::Dist(GetActorLocation(), mRandomMoveInfo.RandomPos) > 100.0f)
	{
		// 目標地点方向を計算
		FVector Direction = (mRandomMoveInfo.RandomPos - GetActorLocation()).GetSafeNormal();

		// 目標地点方向に移動
		FVector NewLocation = GetActorLocation() + Direction * mMoveSpeed * GetWorld()->GetDeltaSeconds();

		// 新しい位置を設定
		SetActorLocation(NewLocation,true);
	}
}

void AEnemyBase::FindAndMoveToLocation(FVector TargetLocation)
{
	// 現在の位置を取�?
	FVector CurrentLocation = GetActorLocation();

	// ターゲ�?ト位置までの距離を計�?
	float Distance = FVector::Dist(TargetLocation, CurrentLocation);

	// 追跡停止�?囲に入ったら処�?を終�?
	if (Distance < mchaseStopDistance)
	{
		UE_LOG(LogTemp, Log, TEXT("Chase stopped. Distance: %f"), Distance);
		return;
	}

	// ターゲ�?ト方向を向くように回転?�?Yaw のみ変更?�?
	FRotator CurrentRotation = GetActorRotation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), mRotationSpeed);
	SetActorRotation(FRotator(CurrentRotation.Pitch, NewRotation.Yaw, CurrentRotation.Roll)); // Yaw のみ更新

	// ナビゲーションシス�?�?を取�?
	UNavigationSystemV1 *NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSystem)
	{
		return;
	}

	// 経路探索を実�?
	UNavigationPath *NavPath = NavSystem->FindPathToLocationSynchronously(GetWorld(), CurrentLocation, TargetLocation);
	if (!NavPath || !NavPath->IsValid() || NavPath->PathPoints.Num() < 2)
	{
		return;
	}

	// 次の目標地点を取�?
	FVector NextPoint = NavPath->PathPoints[1]; // PathPoints[0] は現在地
	NextPoint.Z = CurrentLocation.Z;			// Z軸?��高さ?��を固�?

	// 移動方向と新しい位置を計�?
	FVector Direction = (NextPoint - CurrentLocation).GetSafeNormal();
	FVector NewLocation = CurrentLocation + Direction * mMoveSpeed * GetWorld()->GetDeltaSeconds();
	SetActorLocation(NewLocation);
}

void AEnemyBase::RotateAndObserve()
{
	if (SearchPlayer())
	{
		return; // プレイヤーが見つかった�?�合�?�回転しな�?
	}

	// タイマ�?�を減�?
	mRRInfo.mLookAroundChangeTimer -= GetWorld()->GetDeltaSeconds();

	// 新しい目標角度を設�?
	if (mRRInfo.mLookAroundChangeTimer <= 0.0f)
	{
		// 回転方向をランダ�?に変更
		mRRInfo.mLookAroundDirection = (FMath::RandBool() ? 1 : -1);

		// 現在の角度に対してランダ�?な目標角度を設�?
		float RandomYawOffset = FMath::FRandRange(20.0f, 60.0f) * mRRInfo.mLookAroundDirection;
		mRRInfo.mTargetLookAroundYaw = FMath::Fmod(GetActorRotation().Yaw + RandomYawOffset + 360.0f, 360.0f);

		// タイマ�?�をリセ�?�?
		mRRInfo.mLookAroundChangeTimer = mRRInfo.mLookAroundChangeInterval;
	}

	// 現在の回転を取�?
	FRotator CurrentRotation = GetActorRotation();

	// 補間処�?: 短�?経路を選ぶために角度の差を調整
	float CurrentYaw = CurrentRotation.Yaw;
	float TargetYaw = mRRInfo.mTargetLookAroundYaw;

	// 回転経路の計算（最短経路を選択�?
	float DeltaYaw = TargetYaw - CurrentYaw;
	if (DeltaYaw > 180.0f)
	{
		DeltaYaw -= 360.0f;
	}
	else if (DeltaYaw < -180.0f)
	{
		DeltaYaw += 360.0f;
	}

	// 滑らかな補間
	float NewYaw = CurrentYaw + DeltaYaw * FMath::Clamp(GetWorld()->GetDeltaSeconds() * mRotationSpeed, 0.0f, 1.0f);

	// 回転を適用
	CurrentRotation.Yaw = NewYaw;
	SetActorRotation(CurrentRotation);

	// 目標角度に到達したか確�?
	if (FMath::Abs(DeltaYaw) <= 1.0f) // 誤差を�?慮
	{
		UE_LOG(LogTemp, Log, TEXT("Rotation complete, waiting for next action."));
	}
}

FString AEnemyBase::GetStatus()
{
	{
		float Dist = FVector::Dist(FVector(GetActorLocation().X, GetActorLocation().Y, 0)
			, FVector(mPlayerPos.X, mPlayerPos.Y, 0));
		if (mbCanAttackRadius > Dist && SearchPlayer()) return("Attack");

		switch (mCStatus)
		{
		case ChaseStatus::None:
			return ("None");
		case ChaseStatus::TargetLost:
			return ("TargetLost");
		case ChaseStatus::Idle:
			return ("Idle");
		case ChaseStatus::Chase:
			return ("Chase");
		default:
			return ("");
		}
	}


}

void AEnemyBase::IgnoreSpecificCollision(USphereComponent *TargetCollision)
{
	if (!TargetCollision)
		return;

	TargetCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

bool AEnemyBase::PlayAttack()
{
	return true;
}

// 索敵範囲ヒット時
void AEnemyBase::OnOverlapSearch(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ヒットしたものがPlayerか確認
	APlayerCharacter *pPlayer = Cast<APlayerCharacter>(OtherActor);
	if (pPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("Find Player"));
		mOverlapOtherActor = pPlayer;
		mOverlapOtherComp = OtherComp;
		IsSearchPlayer = true;
	}
}

// 索敵範囲離れた時
void AEnemyBase::OnOverlapSearchEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 離れたものがPlayerか確認
	APlayerCharacter* pPlayer = Cast<APlayerCharacter>(OtherActor);
	if (pPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("loss Player")); 
		mOverlapOtherActor = nullptr;
		mOverlapOtherComp = nullptr;
		IsSearchPlayer = false;
	}
}

void AEnemyBase::OnActorBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
}
