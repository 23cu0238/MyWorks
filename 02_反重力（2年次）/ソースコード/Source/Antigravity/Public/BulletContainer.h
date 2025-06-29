// 発射される弾の全てを格納するクラス
// staticクラス
// なぜかオブジェクトとして配置しなければならないので
// 射撃する際はレベルに配置してください
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.h"
#include "DefaultBullet.h"
#include "Shootable.h"
#include "BulletContainer.generated.h"

UCLASS()
class ANTIGRAVITY_API ABulletContainer : public AActor
{
	// UE固有
	GENERATED_BODY()

public:
	// シングルトンオブジェクトを取得するため static メソッドにする
	static ABulletContainer *GetInstance();
	// 弾を設定する関数
	// 引数1 : 弾の名前・種類
	// 引数2 : 弾を格納する対象のアドレス
	void GetBulletClass(FString tagName, TSubclassOf<class ABullet> &bullet);
	// 弾の状態を設定するクラス
	// 引数1 : 弾の名前・種類
	IShootable *SetupState(FString tagName);

private:
	// 初期化関数
	virtual void BeginPlay() override;

private:
	// コンストラクタ
	ABulletContainer();

private:
	// インスタンスを private static で保持する
	static ABulletContainer *Instance;

private:
	// 弾と名前を結びつける容器
	// ブループリントで登録してください
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TMap<FString, TSubclassOf<class ABullet>> bulletMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<FString, TSubclassOf<UObject>> StatesMap;
};
