// BulletContainerの実装
//詳しくはヘッダーファイルを参照


#include "BulletContainer.h"

// 自身のインスタンスを nullptr で初期化
ABulletContainer* ABulletContainer::Instance = nullptr;


ABulletContainer* ABulletContainer::GetInstance()
{
	// インスタンスがなければ作り、あればそれを返す
	if (Instance == nullptr)
	{
		Instance = NewObject<ABulletContainer>();
		Instance->AddToRoot(); // ガベージコレクション対策
		//作られたことをログに残しとく
		UE_LOG(LogTemp, Error, TEXT("CREATE INCETACE"));
	}
	return Instance;
}

//コンストラクタ
ABulletContainer::ABulletContainer()
{
 	//UE固有
	PrimaryActorTick.bCanEverTick = true;

}

//初期化
void ABulletContainer::BeginPlay()
{
	//UE固有
	Super::BeginPlay();
	//Instanceに自身を登録
	Instance = this;
}
//名前から登録
void ABulletContainer::GetBulletClass(FString tagName, TSubclassOf<class ABullet>& bullet)
{
	//指定された名前の弾が存在しない
	if (bulletMap.Find(tagName) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("tagName IS NULL"));	
	}
	
	if (bulletMap.Find(tagName) != nullptr)
	{
		//指定された弾を押し込む
		bullet = bulletMap[tagName];
		//ログだけ残しておく
		UE_LOG(LogTemp, Log, TEXT("SET BULLET"));
	}
	
}
IShootable* ABulletContainer::SetupState(FString tagName)
{
	if (StatesMap.Find(tagName) == nullptr)
	{
		//ログだけ残しておく
		UE_LOG(LogTemp, Error, TEXT("NOT FOUND NAME"));
		return nullptr;
	}
	IShootable* ret = Cast<IShootable>(NewObject<UObject>(this, *StatesMap.Find(tagName)));
	if (ret)
	{
		//ログだけ残しておく
		UE_LOG(LogTemp, Log, TEXT("SET BULLET STATE"));
	}
	return ret;
}