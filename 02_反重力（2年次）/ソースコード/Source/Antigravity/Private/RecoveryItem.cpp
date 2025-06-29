// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoveryItem.h"
#include "Async/Async.h"

// Sets default values
ARecoveryItem::ARecoveryItem()
	:mItemRotationSpeed(1.f)
	, mItemAddHP(0)
	, mItemAddMP(0)
	, mTilt(15.f)
	,mUpDownSpeed(3.f)
	,mAmplitude(10.f)
	,mBaseHeight(20.f)
	,isHit(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ルートに置くベースオブジェクト生成時
	mpBase = CreateDefaultSubobject<USceneComponent>(TEXT("SceceComp"));
	RootComponent = mpBase;

	//コリジョン判定用ボックスコンポーネント生成
	mpBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollitionComp"));
	mpBoxComp->SetMobility(EComponentMobility::Movable);
	mpBoxComp->SetupAttachment(RootComponent);

	//メッシュコンポーネント(StatickMesh)生成
	mpItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	
	mpItemMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARecoveryItem::BeginPlay()
{
	Super::BeginPlay();
	
	mBaseHeight = GetActorLocation().Z;
}

// Called every frame
void ARecoveryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mpBoxComp->SetWorldLocation(GetActorLocation());
}

//アイテムを回転させる
void ARecoveryItem::RotateItem(float rotateSpeed)
{
	//現在のFPSを測定
	float fps = 1.0 / (GetWorld()->DeltaTimeSeconds);

	//アイテム回転処理
	float rotateCorrection = 60.f / fps;
	FRotator newRot = mpItemMesh->GetRelativeRotation() + FRotator(0, rotateSpeed, 0);
	mpItemMesh->SetRelativeRotation(newRot);
}

//アイテムの角度を傾ける
void ARecoveryItem::TiltItem(float tilt)
{
	FRotator newRot = mpItemMesh->GetRelativeRotation() + FRotator(0, 0, tilt);
	mpItemMesh->SetRelativeRotation(newRot);
}

//アイテムの上下移動
void ARecoveryItem::UpdownAmplitude() 
{
	FVector CurrentLocation = GetActorLocation();

	//浮かぶ動き(sin波を使用)
	float Time = GetWorld()->GetTimeSeconds();
	float Offset = FMath::Sin(Time * mUpDownSpeed) * mAmplitude;

	//新しい位置を決定
	CurrentLocation.Z = mBaseHeight + Offset;

	SetActorLocation(CurrentLocation);
}

void ARecoveryItem::GravityInitialize()
{
	PrimaryActorTick.bCanEverTick = true;

	//初期値設定
	Velocity = FVector::ZeroVector;
	GravityForce = 0.0f;
}

void ARecoveryItem::GravityScale(float DeltaTime)
{
	//重力を速度に加算
	Velocity.Z += GravityForce * DeltaTime;

	//アクターの位置を更新
	AddActorWorldOffset(Velocity * DeltaTime, true);

	//衝突検出のための処理
	FHitResult Hit;

	AddActorWorldOffset(Velocity * DeltaTime, true, &Hit);
	if (Hit.IsValidBlockingHit())
	{
		//衝突していたらZ方向の速度をリセット
		Velocity.Z = 0.0f;
	}
}

//UEがクラッシュしたためAsyncTaskを利用してゲームスレッドで処理するように変更
void ARecoveryItem::SetMesh(UStaticMesh* NewMesh)
{
	if (!NewMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("NewMesh is nullptr."));
		return;
	}

	// this を安全にキャプチャするために TWeakObjectPtr を使用
	TWeakObjectPtr<ARecoveryItem> WeakThis(this);

	AsyncTask(ENamedThreads::GameThread, [WeakThis, NewMesh]()
		{
			if (WeakThis.IsValid() && WeakThis->mpItemMesh)
			{
				WeakThis->mpItemMesh->SetStaticMesh(NewMesh);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ARecoveryItem or mpItemMesh is nullptr in AsyncTask."));
			}
		});
}

void ARecoveryItem::OnOverlapRecovery(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ARecoveryItem::OnOverlapRecoveryEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
