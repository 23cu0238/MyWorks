// ------------------------------------------------
// GravityController.h
// コントローラクラス
// 作成者：李朗曦
// 作成日時：2024-11-28
// ------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GravityController.generated.h"

/**
 * A Player Controller class which adds input-handling functionality for
 * CharacterMovementController's custom gravity mechanics.
 */
UCLASS()
class AGravityController : public APlayerController
{
	GENERATED_BODY()

public:
	// 回転の更新
	virtual void UpdateRotation(float DeltaTime) override;

	// 回転を世界空間から重力相対空間に変換する
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityRelativeRotation(FRotator Rotation, FVector GravityDirection);

	// 重力相対空間から世界空間に回転を変換します
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityWorldRotation(FRotator Rotation, FVector GravityDirection);

private:
	// １フレーム前の重力方向
	FVector LastFrameGravity = FVector::ZeroVector;
};