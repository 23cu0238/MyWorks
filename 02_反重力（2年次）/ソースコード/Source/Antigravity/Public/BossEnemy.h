// Fill out your copyright notice in the Description page of Project Settings.

//--------------------------------------------------------
// BossEnemy.h
// �G�{�X�̎���
// �쐬�ҁF23cu0229 ���쑺 �I�l
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

	// �ړ��X�V����
	void UpdateMove(float DeltaTime) override;

	// �U���̃N�[���^�C��
	UPROPERTY(EditAnywhere, Category = "Enemy")
		FTimerHandle mAttackTimerHandle;
	// �U���̏I�������p�̃^�C�}�[
	FTimerHandle mAttackEndTimerHandle;

	// �U��
	void BossAttack();
	// �ߐڍU��
	void CloseAttack();
	// �ːi�U��
	void RushAttack();
	// �U���I���֐�
	void EndAttack();

	// �ǂɂԂ���
	//UFUNCTION()
	//void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// �ːi�I��
	void CheckRushEnd();
	// �ːi�I���\����W
	FVector mRushTargetLocation;
	// �ːi�̖ړI�n�m�F�����I�ɍs��
	FTimerHandle mRushCheckTimerHandle;

	// �N�[���^�C�������Z�b�g���ꂽ
	void ResetAttackCooldown();

	// �v���C���[�̈ʒu���L�^
	FVector PlayerPosition;

	// �ːi���x
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
	// Boss�̃��b�V��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UStaticMeshComponent> mBossMesh;

	// �ߐڍU���͈�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UBoxComponent> mCloseAttackComp;

private:
	// �ˌ��N���X
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
