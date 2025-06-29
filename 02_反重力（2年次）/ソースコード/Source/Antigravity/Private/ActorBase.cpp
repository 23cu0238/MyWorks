// ------------------------------------------------
// ActorBase.cpp
// キャラクターの基本クラス
// 作成者：李朗曦、浅川皓亮
// 作成日時：2024-11-18
// ------------------------------------------------

#include "ActorBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// 　コンストラクタ
AActorBase::AActorBase(const FObjectInitializer &ObjectInitializer)
{
}

// スタート関数
void AActorBase::BeginPlay()
{
	// 親クラスのBeginPlay()
	Super::BeginPlay();

	// 初期化
	Init();

	// 無敵時間を設定
	mDamageCD = 2.0f;
}

// 毎フレーム更新
void AActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Update();

	if (mDamageTimer > 0)
	{
		mDamageTimer = UKismetMathLibrary::FClamp(mDamageTimer - GetWorld()->GetDeltaSeconds(), 0.0f, mDamageCD);
	}
}

// 初期化
void AActorBase::Init()
{
}

// 死亡判定関数
bool AActorBase::Dead()
{
	return false;
}

// HP減らす関数
void AActorBase::FluctuationHP(float decrease)
{
	// 無敵時間中であれば処理しない
	if (mDamageTimer > 0.0f)
	{
		return;
	}

	// HPを減らす
	mHitPoint -= decrease;
	// 無敵時間をカウント
	mDamageTimer = mDamageCD;

	// デバッグ用
	UE_LOG(LogTemp, Log, TEXT("OBJECT HP IS %d"), mHitPoint);
}
