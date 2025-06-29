#include "IDeadable.h"

UDeadable::UDeadable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool IDeadable::Dead() {
	return false;
}

void IDeadable::FluctuationHP(float decrease) {

}

