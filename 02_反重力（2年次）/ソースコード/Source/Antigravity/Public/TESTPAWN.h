//簡単なメモ書きのみ

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include"GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "TESTPAWN.generated.h"

class UShootingAttack;

UCLASS()
class ANTIGRAVITY_API ATESTPAWN : public ACharacter
{
	GENERATED_BODY()

public:
	//コンストラクタ
	ATESTPAWN();

protected:
	//初期化
	virtual void BeginPlay() override;

public:
	// 実行関数
	virtual void Tick(float DeltaTime) override;
	// 未使用　よくわからん
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	//射撃関数
	void Shoot();
	//射撃関数
	void ShootMissile();
	//触れた対象をTARGETとして設定される
	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* HitCom, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
private:
	//攻撃のコンポーネント
	UPROPERTY()
	UShootingAttack* shoot;
	UPROPERTY()
	UShootingAttack* missile;
	//対象を射撃する範囲
	float radius = 300.0f;
	//くーるたいむ　
	//減算式
	float timer = 0;
	float time = 10;
	//射撃対象を格納する変数
	UPROPERTY()
	AActor* mTargetActor;
	//衝撃を加えるため
	UPROPERTY()
	UPrimitiveComponent* mTargetComp;
};
