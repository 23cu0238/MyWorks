//	 重力を変更するのに使います
//	β版なのでクレームは受け付けません
//	Componentクラス

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChangeGravityComponent.generated.h"

class ACharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ANTIGRAVITY_API UChangeGravityComponent : public UActorComponent
{
	// UE固有
	GENERATED_BODY()

public:
	// コンストラクタ
	UChangeGravityComponent();

protected:
	// 初期化
	virtual void BeginPlay() override;

public:
	// 重力を変える対象を設定します
	// 引数1 : Targetのアドレス
	void SetTarget(ACharacter *target);
	// 重力を変更する実行関数
	// 引数1 : 重力の方向
	void ChangeGravity(const FHitResult &gravityPoint);
	// 変更し終わったら呼ぶかも
	void OnExit();

private:
	// 重力を変更させる対象
	ACharacter *mTarget;
};
