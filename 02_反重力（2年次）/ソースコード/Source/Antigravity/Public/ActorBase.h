// ------------------------------------------------
// ActorBase.h
// キャラクターの基本クラス
// 作成者：李朗曦、浅川皓亮
// 作成日時：2024-11-18
// ------------------------------------------------

#pragma once
#include "IDeadable.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActorBase.generated.h"

// 前方宣言
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ANTIGRAVITY_API AActorBase : public ACharacter, public IDeadable
{
	GENERATED_BODY()

public:
	// コンストラクタ
	AActorBase(const FObjectInitializer &ObjectInitializer);

protected:
	// スタート関数
	virtual void BeginPlay() override;

public:
	// 初期化
	virtual void Init();
	// 実行関数
	virtual void Update() {};

public:
	// 毎フレーム更新
	virtual void Tick(float DeltaTime) override;

	// 死亡判定関数
	bool Dead() override;

	// HP減らす関数
	void FluctuationHP(float decrease) override;

	// HPを取得用
	UFUNCTION(BlueprintCallable)
	int GetHP() const { return mHitPoint; }

protected:
	// 密度
	float mMass;
	// 移動速度
	float mMoveSpeed;
	// 無敵時間タイマー
	float mDamageTimer;
	// 無敵時間
	float mDamageCD;
	// 体力
	UPROPERTY(BlueprintReadWrite, Category = Parameter, meta = (AllowPrivateAccess = "true"))
	int mHitPoint;
};