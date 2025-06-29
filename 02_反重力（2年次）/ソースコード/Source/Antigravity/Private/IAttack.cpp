#include"IAttack.h"
#include"IDeadable.h"


UAttackable::UAttackable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void IAttackable::Attack(float damage, AActor* OtherActor, FVector position)
{

}