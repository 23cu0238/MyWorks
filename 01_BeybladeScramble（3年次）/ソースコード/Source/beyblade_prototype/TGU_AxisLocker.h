// アクターの物理コンストレイントを設定するコンポーネントプラグイン
// ソース：https://sourceforge.net/projects/ue4-blueprint-axis-locker/files/

// TGU_AxisLocker is an Actor Component used to set the constraint mode for actor components, since Blueprints cannot do that.
// Attach TGU_AxisLocker to the component you want to affect. If the parent component is ineligible, you'll get an error of BeginPlay().
// If you want to support the author, check out The GoD Unit game on Steam =)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TGU_AxisLocker.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTGU_AxisLocker : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTGU_AxisLocker();

	FBodyInstance* BodyInstance;

	// Set new constraint mode of the affected component
	UFUNCTION(BlueprintCallable)
	void SetNewConstraintMode(bool LockPositionX, bool LockPositionY, bool LockPositionZ, bool LockRotationX, bool LockRotationY, bool LockRotationZ);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Try and get hold of the affected component's Body Instance on BeginPlay()
	void GetBodyInstance();
	bool bIsBodyInstanceValid = false;
};
