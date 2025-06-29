#include "Missile.h"
#include "InductionState.h"
#include "ChargeState.h"
#include "ExplosionState.h"
#include "MissileState.h"


//コンストラクタ
UMissile::UMissile()
{
	//PrimaryComponentTick.bCanEverTick = true;
}

void UMissile::InitState()
{
	//inducnt = NewObject<UInductionState>();
	//charge = NewObject<UChargeState>();
	//explosion = NewObject < UExplosionState>();
}

ABullet* UMissile::GetOwnerBullet()const
{
	return mBullet;
}

void UMissile::Init()
{
	//mBullet = Cast<ABullet>(GetOwner());
	//
	//if (!currentState)
	//	currentState = NewObject<UInductionState>();

	//currentState->OnEnter(this);
}
void UMissile::Update(float DeltaTime)
{
	
}


void UMissile::ChangeState(IMissileState* nextState)
{
	//if (!nextState)
	//	return;
	//if (!currentState)
	//	return;

	//currentState->OnExit(this);
	//currentState = nextState;
}

TArray<FVector> UMissile::GetAnchorPoint()const
{
	return anchorPoint;
}
