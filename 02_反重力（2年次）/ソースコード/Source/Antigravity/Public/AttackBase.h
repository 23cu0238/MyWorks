
#pragma once

#include "CoreMinimal.h"
#include "AttackBase.generated.h"

// 
// �N���X���FAttackBase�N���X
// �p�r�FAttack�̊��N���X�Ƃ���
// �쐬�ҁF23cu0216���n����
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
	virtual void Attack(float power);	// �U���֐�

public:
	UPROPERTY(EditAnywhere, Category = "Attack") int mPower;	// ���̍U�����̂̍U����
};
