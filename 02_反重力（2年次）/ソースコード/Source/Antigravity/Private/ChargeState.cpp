// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargeState.h"
#include"Missile.h"
#include "Kismet/GameplayStatics.h"


UChargeState::UChargeState()
{

}
void UChargeState::OnEnter(UMissile* owner)
{
	ChargeTimer = 5;
}
void UChargeState::OnUpdate(UMissile* owner)
{
	ChargeTimer -= GetWorld()->GetDeltaSeconds();

	if (ChargeTimer > 0)
		return;
	if (IMissileState* im = Cast<IMissileState>(mOwner->charge))
		mOwner->ChangeState(im);
}
void UChargeState::OnExit(UMissile* owner)
{

}
