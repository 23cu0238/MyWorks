// Fill out your copyright notice in the Description page of Project Settings.


#include "SavePoint.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

//静的メンバー変数の定義
FVector
ASavePoint::RespawnLocation = FVector::ZeroVector;

// Sets default values
ASavePoint::ASavePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // コリジョンボックスのセットアップ
    UBoxComponent* CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    //コリジョンボックスのサイズを拡大
//<<<<<<< HEAD
    CollisionBox->SetBoxExtent(FVector(300.0f, 300.0f, 300.0f));    //範囲調整
//=======
    CollisionBox->SetBoxExtent(FVector(300.0f, 300.0f, 300.0f));    //範囲を広くする
//>>>>>>> f406533f227dcf8ab0fba6555e2bef168b536ab2

    // コリジョンイベントをバインド
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASavePoint::OnOverlapBegin);

    bIsActive = false; // 初期状態は非アクティブ
}

// Called when the game starts or when spawned
void ASavePoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASavePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASavePoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("Player"))
    {
        // 他のセーブポイントをリセット
        for (TActorIterator<ASavePoint> It(GetWorld()); It; ++It)
        {
            It->bIsActive = false;
        }

        // このセーブポイントをアクティブに設定
        bIsActive = true;

        // リスポーン地点を更新
        RespawnLocation = GetActorLocation();
        UE_LOG(LogTemp, Log, TEXT("SavePoint Activated at: %s"), *RespawnLocation.ToString());

    }
}