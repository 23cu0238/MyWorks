// ------------------------------------------------
// GravityCharacterMovementComp.h
// 重力キャラクターのムーブメントコンポーネントクラス
// 作成者：李朗曦
// 作成日時：2024-12-01
// ------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GravityCharacterMovementComp.generated.h"

// 前方宣言
class ACharacter;

UCLASS()
class ANTIGRAVITY_API UGravityCharacterMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	/** 毎フレーム更新するときローテーションも更新 */
	void PhysicsRotation(float DeltaTime) override;

	// スタート関数
	virtual void BeginPlay() override;

	// 毎フレーム更新
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	// 回転量の差を取得
	UFUNCTION(BlueprintCallable)
	FRotator GetDeltaRot() const { return mDeltaRot; }

	// 重力方向を変更
	UFUNCTION(BlueprintCallable)
	void SetGravityDirP(const FVector &GravityDir) { SetGravityDirection(GravityDir); }

	// 地面に垂直すべきか？
	UFUNCTION(BlueprintCallable)
	bool GetShouldRemainVertical() const
	{
		if (!bRemainVertical)
		{
			return false;
		}

		else
		{
			return ShouldRemainVertical();
		}
	}

private:
	// 初期の回転率
	FRotator mOriginalRotationRate;

	// 地面に垂直すべきかを設定用変数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gravity, meta = (AllowPrivateAccess = "true"))
	bool bRemainVertical;

	// 回転量の差
	FRotator mDeltaRot;
};
