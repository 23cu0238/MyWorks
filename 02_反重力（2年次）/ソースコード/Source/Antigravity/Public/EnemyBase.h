// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IDeadable.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacterDef.h"
#include "EnemyBase.generated.h"

class USphereComponent;
class UBoxComponent;
class ConventionalAttack;
class AEventArea;

UCLASS()
class ANTIGRAVITY_API AEnemyBase : public ACharacter, public IDeadable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool Dead() override;

	UFUNCTION(BlueprintCallable)
	void FluctuationHP(float decrease) override;

	UFUNCTION(BlueprintCallable)
	float GetHP() const { return HP; }

	//UFUNCTION(/*BlueprintCallable*/)
	void SetmonsterHouse(AEventArea& pmonsterhouse) { mMonsterHouse = &pmonsterhouse; }

	UFUNCTION(BlueprintCallable)
	FString GetMonsterHouseID() { return mMonsterHouseID; }

	// 経路探索
	void FindAndMoveToLocation(FVector TargetLocation);
protected:
	virtual void UpdateMove(float DeltaTime);

	// ���G����
	// ���ʂ�bool�^�ŕԂ�
	UFUNCTION(BlueprintCallable)
	virtual bool SearchPlayer()const { return IsSearchPlayer; }

	// ランダ�?移動�?��?
	void RandomMove();


	void RotateAndObserve();

	// 攻�?
	virtual bool PlayAttack();

protected:

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float mMoveSpeed; // ?��ړ�?��?��?��x

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float mRotationSpeed; // 回転速度

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float HP; // ?��ړ�?��?��?��x
	UPROPERTY(EditAnywhere, Category = "Enemy")
	float Power; // ?��ړ�?��?��?��x

	UPROPERTY(EditAnywhere, Category = "Enemy")
	TObjectPtr<AEventArea> mMonsterHouse;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	FString mMonsterHouseID;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	TObjectPtr<UBoxComponent> mSearchComp;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	bool IsSearchPlayer;

	// 索敵範囲のオーバーラップ
	UFUNCTION(BlueprintCallable)
	void OnOverlapSearch(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnOverlapSearchEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	// 警戒範囲
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> mAlertComp;

	// 警戒半径
	UPROPERTY(EditAnywhere, Category = "Alert")
	float mAlertRadius;

	// ヒット
	UFUNCTION(BlueprintCallable)
	void OnActorBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	bool bIsPlayerInAlertRange;
protected:
	ChaseStatus mCStatus; // 追跡状�?
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector mPlayerPos; // Player座�?

	//
	// 追跡情報
	//
	UPROPERTY(EditAnywhere, Category = "Chase")
	float mchaseStopDistance; // 追跡停止距離

	float mMovementinterval; // ランダ�?移動時の移動間�?
	FVector mInitPos;		 // 初期位置

protected:
	//
	// ランダ�?移動情報
	//
	RandomRotationInfo mRRInfo;		// ランダ�?回転用�?報
	RandomMoveInfo mRandomMoveInfo; // ランダ�?移動情報

	//
	// 見失�?�?報
	//
	UPROPERTY(EditAnywhere, Category = "TargetLost")
	float mLostTime;	// 見失�?時間
	float mLostnowTime; // 見失った経過時間
	bool IsLostTarget;	// targetを見失ったか
	bool bIsTargetOverlappingAttack = false;
	FVector mPlayerLastPos;		// 見つけた最後�?�座�?
	FVector mLastKnownPosition; // ターゲ�?ト�?�最後に観測された位置
	bool bHasLastKnownPosition; // 最後に観測された位置を記録して�?るかど�?�?

	AActor *mOverlapOtherActor;
	UPrimitiveComponent *mOverlapOtherComp;

	RandomRotateInfo mRandomrotateinfo;


	// �U���͈�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> mAttackComp;

	// �U���֌W
	bool mIsActiveAttack;	// �U���A�j���[�V�����p�t���O
	UPROPERTY(EditAnywhere)
	float mbCanAttackRadius;	// �U���\�͈�
	bool mbCanAttack;	// �U���\��

	float mDeadTimer;

	public:
		UFUNCTION(BlueprintCallable)
		virtual FString GetStatus();

		// ステータス設定
		void SetStatus(const ChaseStatus c = ChaseStatus::None) { mCStatus = c; } ;

	UFUNCTION(BlueprintCallable)
	void IgnoreSpecificCollision(USphereComponent* TargetCollision);
};
