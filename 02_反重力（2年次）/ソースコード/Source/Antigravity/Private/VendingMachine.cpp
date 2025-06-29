// Fill out your copyright notice in the Description page of Project Settings.

#include "VendingMachine.h"
#include "RecoveryHp.h" // スポーンするアクタークラス(HP)
#include "RecoveryMp.h" // スポーンするアクタークラス(MP)
#include "RecoveryItem.h"
#include "PlayerCharacter.h" // プレイヤー関連のヘッダーをインクルード

// Sets default values
AVendingMachine::AVendingMachine()

{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// メッシュコンポーネント(StatickMesh)生成
	mVendingMachineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VendingMachineMesh"));
	mVendingMachineMesh->SetupAttachment(RootComponent);

	// HPの初期値を設定
	mHealth = 100.0f; // 自動販売機のHPを100に設定
}

// Called when the game starts or when spawned
void AVendingMachine::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;

	// 重力に関する初期値設定
	Velocity = FVector::ZeroVector;
	GravityForce = -980.0f;

	// 仮にここに置いている
	SpawnActors();
}

// 毎フレームの呼び出し
void AVendingMachine::Tick(float DeltaTime)
{
	// 重力を速度に加算
	Velocity.Z += GravityForce * DeltaTime;

	// アクターの位置を更新
	AddActorWorldOffset(Velocity * DeltaTime, true);

	// 衝突検出のための処理
	FHitResult Hit;

	AddActorWorldOffset(Velocity * DeltaTime, true, &Hit);
	if (Hit.IsValidBlockingHit())
	{
		// 衝突していたらZ方向の速度をリセット
		Velocity.Z = 0.0f;
	}

	switch (mStatus)
	{
		// 始め
	case ITEM_STATUS_NONE:
	{
		// 異常なし
		UE_LOG(LogTemp, Log, TEXT("NONE"));
	}
	break;
	// 重力を加えられた状態(使用しない可能あり)
	case ITEM_STATUS_ADDGRAVITY:
	{
	}
	break;
	// 自身を破壊状態へ移行する
	case ITEM_STATUS_DESTROY_TRANSITION:
	{
		// アイテムを落とす
		SpawnActors();
		UE_LOG(LogTemp, Log, TEXT("TRANSITION"));
		// mStatus = ITEM_STATUS_DESTROY;		//破壊状態に
	}
	break;
	// 自身を破壊状態に
	case ITEM_STATUS_DESTROY:
	{
		// 自分を粉々にする(今は消す)
		this->Destroy();

		// 通常状態へ
		mStatus = ITEM_STATUS_NONE;
	}
	break;
		// 終わり
	}
}

void AVendingMachine::SpawnActors()
{
	// 自分の座標を取得
	FVector MyPosition = GetActorLocation();

	// スポーン位置を少しずらして設定
	FVector Offset1(100.f, 0.f, 0.f); // X方向に100ユニット
	FVector Offset2(0.f, 100.f, 0.f); // Y方向に100ユニット

	FVector SpawnPosition1 = MyPosition + Offset1;
	FVector SpawnPosition2 = MyPosition + Offset2;

	// ワールドの取得
	UWorld *World = GetWorld();
	if (World)
	{
		// RecoveryHpをスポーンしてメッシュを設定
		ARecoveryHp *RecoveryHp = World->SpawnActor<ARecoveryHp>(ARecoveryHp::StaticClass(), SpawnPosition1, FRotator::ZeroRotator);
		if (RecoveryHp && RecoveryHp->Mesh1)
		{
			RecoveryHp->SetMesh(RecoveryHp->Mesh1);
		}

		else
		{
			UE_LOG(LogTemp, Error, TEXT("RecoveryHp or Mesh1 is NULL"));
		}

		// RecoveryMpをスポーンしてメッシュを設定
		ARecoveryMp *RecoveryMp = World->SpawnActor<ARecoveryMp>(ARecoveryMp::StaticClass(), SpawnPosition2, FRotator::ZeroRotator);
		if (RecoveryMp && RecoveryMp->Mesh2)
		{
			RecoveryMp->SetMesh(RecoveryMp->Mesh2);
		}

		else
		{
			UE_LOG(LogTemp, Error, TEXT("RecoveryMp or Mesh2 is NULL"));
		}
	}
}

// HPを減少させる関数
void AVendingMachine::FluctuationHP(float Decrease)
{
	// HPを減少
	mHealth -= Decrease;
	/*UE_LOG(LogTemp, Log, TEXT("Vending Machine HP: %f"), Health);*/

	// HPがゼロ以下の場合、破壊状態へ移行
	if (mHealth <= 0)
	{
		mStatus = ITEM_STATUS_DESTROY_TRANSITION; // 状態を破壊移行状態に設定
		UE_LOG(LogTemp, Warning, TEXT("Vending Machine is Destroyed"));
	}
}

bool AVendingMachine::Dead()
{
	return mHealth <= 0; // HPが0以下ならtrueを返す
}