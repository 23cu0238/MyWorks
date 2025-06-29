// Fill out your copyright notice in the Description page of Project Settings.

//--------------------------------------------------------
// BossEnemy.h
// 敵ボスの実装
// 作成者：23cu0229 日野村 悠人
//--------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "AttackBase.h"
#include "PlayerCharacter.h"
#include "PlayerAttack.h"
#include "GameFramework/Character.h"
#include "BossEnemy.generated.h"


class UShootingAttack;

/**
 *
 */
UCLASS()
class ANTIGRAVITY_API ABossEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossEnemy();
	~ABossEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 移動更新処理
	void UpdateMove(float DeltaTime) override;

	// 攻撃のクールタイム
	UPROPERTY(EditAnywhere, Category = "Enemy")
		FTimerHandle mAttackTimerHandle;
	// 攻撃の終了処理用のタイマー
	FTimerHandle mAttackEndTimerHandle;

	// 攻撃
	void BossAttack();
	// 近接攻撃
	void CloseAttack();
	// 突進攻撃
	void RushAttack();
	// 攻撃終了関数
	void EndAttack();

	// 壁にぶつかる
	//UFUNCTION()
	//void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// 突進終了
	void CheckRushEnd();
	// 突進終了予定座標
	FVector mRushTargetLocation;
	// 突進の目的地確認を定期的に行う
	FTimerHandle mRushCheckTimerHandle;

	// クールタイムがリセットされた
	void ResetAttackCooldown();

	// プレイヤーの位置を記録
	FVector PlayerPosition;

	// 突進速度
	UPROPERTY(EditAnywhere, Category = "Enemy")
		float mRushSpeed;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	/*virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;*/

private:
	UFUNCTION(BlueprintCallable)
		void OnOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void AttackFunction(AActor* OtherActor, UPrimitiveComponent* OtherComp);

	UFUNCTION(BlueprintCallable)
		void OnOverlapAttackEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//void PlayAttack() override;

public:
	UFUNCTION(BlueprintCallable)
		void SetActiveAttack(bool isActive) { mIsActiveAttack = isActive; }

private:
	// Bossのメッシュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UStaticMeshComponent> mBossMesh;

	// 近接攻撃範囲
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UBoxComponent> mCloseAttackComp;

private:
	// 射撃クラス
	UPROPERTY(EditAnywhere, Category = "EnemyAttack")
	UShootingAttack* mShootiongAttack;

	UPROPERTY(EditAnywhere, Category = "EnemyAttack")
	FString mShootingName = "Normal";

	UPROPERTY(EditAnywhere, Category = "EnemyAttack")
	float mShootingRadius;

	UPROPERTY(EditAnywhere, Category = "EnemyAttack")
	float mShootingCoolTime;

	UPROPERTY(EditAnywhere, Category = "EnemyAttack")
	float mShootingTime;

	TObjectPtr<USphereComponent> mShootingRange;

	UPROPERTY(EditAnywhere)
		float UpPosZ;

public:
		FString GetStatus()override;

};
