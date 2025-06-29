// ShootingAttackの実�?
// 詳しいことはヘッダーファイル参�?�

#include "ShootingAttack.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "BulletContainer.h"

// 初期化関数
UShootingAttack::UShootingAttack()
{

}

// 攻�?
void UShootingAttack::Attack(float damage, AActor *target, FVector Start)
{
	// TARGETの位置
	FVector End = target->GetActorLocation();
	// 方�?
	FRotator direction = (End - Start).Rotation();
	if (!BulletClass)
		Init(mOwner, mRadius, pWorld, BulletName);

	// レベルが登録されて�?なければログを�?��?
	// �?荷掛かるから、tryはしな�?
	if (pWorld == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("WORLD IS NULL"));
	}
	// 弾が登録されて�?れ�?�発�?
	if (BulletClass != nullptr)
	{
		// 構�?体作�??
		FActorSpawnParameters ActorSpawnParams;
		// 生�?�ルール�?�?
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// 念のため、ログを�?�しておきま�?
		UE_LOG(LogTemp, Log, TEXT("ShootingAttack"));
		// 生�??
		ABullet *a = pWorld->SpawnActor<ABullet>(BulletClass, Start, direction, ActorSpawnParams);
		//ABullet* a = pWorld->SpawnActor<ABullet>(BulletClass/*, Start, direction, ActorSpawnParams*/);
		a->SetOriginator(GetOwner());
	}
	// 弾がな�?で�?
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unloaded"));
	}
}

// 初期�?
void UShootingAttack::Init(AActor *owner, float radius, UWorld *world, FString tag)
{
	mOwner = owner;
	mRadius = radius;
	pWorld = world;
	BulletName = tag;
	UMyGameInstance *instance = Cast<UMyGameInstance>(pWorld->GetGameInstance());
	if (instance != nullptr)
	{
		ABulletContainer *a = instance->GetBulletContainer();
		instance->GetBulletContainer();
		if (a)
		{
			a->GetBulletClass(BulletName, BulletClass);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMyGameInstance IS NULL"));
	}
}
