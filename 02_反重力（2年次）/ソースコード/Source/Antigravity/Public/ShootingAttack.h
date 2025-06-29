// 射撃攻撃の実装
//呼ぶだけで勝手に攻撃してくれます

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/ActorComponent.h"
#include "IAttack.h"
#include "Bullet.h"
#include "IAttack.h"
#include "ShootingAttack.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ANTIGRAVITY_API UShootingAttack : public UActorComponent, public IAttackable
{
	//UE固有
	GENERATED_BODY()

public:
	//コンストラクタ
	UShootingAttack();
	//発射する弾のクラス
	//弾はすべてコンテナに格納されているので、そこから取ってください
	TSubclassOf<class ABullet> BulletClass;
	//初期化
	//引数1 : このコンポーネントを保持するActor
	//引数2 : 攻撃する半径
	//引数3 : オブジェクトが存在するレベル
	//引数4 : 出力する弾の種類
	void Init(AActor* owner, float radius , UWorld* world, FString tag);
	//攻撃の実行関数
	//引数1 : オーナーの攻撃力
	//引数2 : 攻撃するTARGET
	//引数3 : 攻撃の発生位置
	void Attack(float damage, AActor* target,FVector position)override;

private:
	//出力する弾の種類 名前
	FString BulletName;
	//オーナーを保持するための変数
	UPROPERTY()
	AActor* mOwner;
	//オーナーが存在するレベル
	UPROPERTY()
	UWorld* pWorld;
	//攻撃する半径
	float mRadius;
};
