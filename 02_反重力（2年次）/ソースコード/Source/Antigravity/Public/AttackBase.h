
#pragma once

#include "CoreMinimal.h"
#include "AttackBase.generated.h"

// 
// クラス名：AttackBaseクラス
// 用途：Attackの基底クラスとして
// 作成者：23cu0216相馬理玖
// 

UCLASS()
class ANTIGRAVITY_API AttackBase : public APawn
{
	GENERATED_BODY()
public:
	AttackBase();
	virtual ~AttackBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void Attack(float power);	// 攻撃関数

public:
	UPROPERTY(EditAnywhere, Category = "Attack") int mPower;	// この攻撃自体の攻撃力
};
