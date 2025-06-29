// Fill out your copyright notice in the Description page of Project Settings.

#include "InductionState.h"
#include "Missile.h"
#include "PlayerAttack.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Bullet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

UInductionState::UInductionState()
{
}

void UInductionState::OnEnter(UMissile *owner)
{	
	//�U���������
	//mAttack = NewObject<UConventionalAttack>();
	//�I�[�i�[�̏���ݒ�
	mOwner = owner;
	//�ŏ��̈ʒu��擾
	startPosition = GetOwner()->GetActorLocation();
}
void UInductionState::OnUpdate(UMissile *owner)
{
	//�ړ�
	Induction();
	//�ړI�n�ɋ߂Â��Ă��邩
	if (!IsReachedEnd())
		return;
	//��Ԃ�`���[�W�Ɉڍs
	 if(IMissileState* im = Cast<IMissileState>(mOwner->charge))
	 	mOwner->ChangeState(im);
}
void UInductionState::OnExit(UMissile *owner)
{
}

void UInductionState::OnHit(UMissile *owner, AActor *HitActor, UPrimitiveComponent HitComp, const FHitResult &Hit)
{
	// �q�b�g�����I�u�W�F�N�g�i�[�p
	FHitResult OutHits;
	//���������@����擾
	owner->HitNormal = -1 * UKismetMathLibrary::GetDirectionUnitVector(OutHits.ImpactNormal * 100.0f + OutHits.ImpactPoint, OutHits.ImpactPoint);
	owner->SpawnRotation = FRotationMatrix::MakeFromZ(owner->HitNormal).Rotator();
}

void UInductionState::Induction()
{
	timer += GetWorld()->GetDeltaSeconds();
	GetOwner()->SetActorLocation(Evaluate(timer));
}

bool UInductionState::IsReachedEnd()
{
	//if (GetOwner()->GetActorLocation().X - mOwner->GetOwnerBullet()->GetEnd().X > 1.0f)
	//	return false;
	//if (GetOwner()->GetActorLocation().Y - mOwner->GetOwnerBullet()->GetEnd().Y > 1.0f)
	//	return false;
	//if (GetOwner()->GetActorLocation().Z - mOwner->GetOwnerBullet()->GetEnd().Z > 1.0f)
	//	return false;

	return true;
}

/// <summary>
/// �x�W�F�Ȑ��֐�
/// </summary>
FVector UInductionState::Evaluate(float t)
{
	if (mOwner->GetAnchorPoint().Num() == 0)
	{
		return FVector(0, 0, 0);
	}

	FVector result;

	if (count > mOwner->GetAnchorPoint().Num())
	{
		count = 0;
	}
	//result = (FMath::Pow((1 - t), 3) * Start) + 3 * (1 - t) * (FMath::Pow(t, 2) * mOwner->GetAnchorPoint()[count]) + (FMath::Pow(t, 3) * mOwner->GetOwnerBullet()->GetEnd());

	return result;
}
