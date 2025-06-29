// アクターの物理コンストレイントを設定するコンポーネントプラグイン
// ソース：https://sourceforge.net/projects/ue4-blueprint-axis-locker/files/

// TGU_AxisLocker is an Actor Component used to set the constraint mode for actor components, since Blueprints cannot do that.
// Attach TGU_AxisLocker to the component you want to affect. If the parent component is ineligible, you'll get an error of BeginPlay().
// If you want to support the author, check out The GoD Unit game on Steam =)

#include "TGU_AxisLocker.h"

// Sets default values for this component's properties
UTGU_AxisLocker::UTGU_AxisLocker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UTGU_AxisLocker::BeginPlay()
{
	Super::BeginPlay();
	GetBodyInstance();
}

void UTGU_AxisLocker::GetBodyInstance()
{
	UPrimitiveComponent *Primitive = Cast<UPrimitiveComponent>(GetAttachParent());
	if (Primitive != nullptr)
	{
		BodyInstance = Primitive->GetBodyInstance();
		if (BodyInstance != nullptr)
		{
			bIsBodyInstanceValid = true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, "AXIS LOCKER: AFFECTED COMPONENT INVALID. PLEASE REPARENT AXIS LOCKER TO ANOTHER COMPONENT");
			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, "AXIS LOCKER: AFFECTED COMPONENT INVALID. PLEASE REPARENT AXIS LOCKER TO ANOTHER COMPONENT");
			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, "AXIS LOCKER: AFFECTED COMPONENT INVALID. PLEASE REPARENT AXIS LOCKER TO ANOTHER COMPONENT");
			UE_LOG(LogTemp, Error, TEXT("AXIS LOCKER: AFFECTED COMPONENT INVALID. PLEASE REPARENT AXIS LOCKER TO ANOTHER COMPONENT"));
			UE_LOG(LogTemp, Error, TEXT("AXIS LOCKER: AFFECTED COMPONENT INVALID. PLEASE REPARENT AXIS LOCKER TO ANOTHER COMPONENT"));
			UE_LOG(LogTemp, Error, TEXT("AXIS LOCKER: AFFECTED COMPONENT INVALID. PLEASE REPARENT AXIS LOCKER TO ANOTHER COMPONENT"));
		}
	}
}

void UTGU_AxisLocker::SetNewConstraintMode(bool LockPositionX, bool LockPositionY, bool LockPositionZ, bool LockRotationX, bool LockRotationY, bool LockRotationZ)
{
	if (!bIsBodyInstanceValid)
		return;

	BodyInstance->bLockXTranslation = LockPositionX;
	BodyInstance->bLockYTranslation = LockPositionY;
	BodyInstance->bLockZTranslation = LockPositionZ;
	BodyInstance->bLockXRotation = LockRotationX;
	BodyInstance->bLockYRotation = LockRotationY;
	BodyInstance->bLockZRotation = LockRotationZ;
	BodyInstance->SetDOFLock(EDOFMode::SixDOF);
}