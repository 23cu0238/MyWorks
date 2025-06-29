// Fill out your copyright notice in the Description page of Project Settings.

#include "FallDamageObject.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFallDamageObject::AFallDamageObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//デフォルト値の設定
	FallTime = 0.0f;
	FallThreshold = 0.2f;  // 0.2秒以上落下したらダメージ
	DamageAmount = 50.0f;   // ダメージ量
	bIsFalling = false;
}

// Called when the game starts or when spawned
void AFallDamageObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFallDamageObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFalling)
	{
		FallTime += DeltaTime;  // 落下している時間を加算
		if (FallTime >= FallThreshold)
		{
			ApplyFallDamage();  // ダメージを与える
			bIsFalling = false;  // 1回だけダメージを与えるようにする
		}
	}
}

// 落下時間が経過した場合にダメージを与える
void AFallDamageObject::ApplyFallDamage()
{
    // ワールド内の全てのアクターと衝突を確認するためのスフィアキャスト
    FVector ActorLocation = GetActorLocation(); // このアクターの現在の位置
    float Radius = 200.0f; // ダメージを与える範囲の半径

    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); // 自分自身を無視

    // スフィアの衝突判定
    bool bOverlap = GetWorld()->OverlapMultiByObjectType(
        OverlapResults,
        ActorLocation,
        FQuat::Identity,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn), // Pawn チャンネルを指定（敵は通常このチャンネル）
        FCollisionShape::MakeSphere(Radius),
        QueryParams
    );

    if (bOverlap)
    {
        for (const FOverlapResult& Result : OverlapResults)
        {
            AActor* HitActor = Result.GetActor(); // 衝突したアクターを取得

            if (HitActor && HitActor->ActorHasTag(TEXT("Enemy"))) // Enemy タグがついたアクターを判定
            {
                // ダメージを適用
                UGameplayStatics::ApplyDamage(
                    HitActor,                   // ダメージを与える対象
                    DamageAmount,               // ダメージ量
                    GetInstigatorController(),  // このダメージを与えたコントローラー
                    this,                       // ダメージの原因となったアクター
                    nullptr                     // ダメージタイプ（デフォルトで null）
                );

                UE_LOG(LogTemp, Warning, TEXT("Applied %f damage to %s"), DamageAmount, *HitActor->GetName());
            }
        }
    }
	UE_LOG(LogTemp, Warning, TEXT("Fall damage applied: %f"), DamageAmount);
}

// 落下中かどうかを設定
void AFallDamageObject::StartFalling()
{
	bIsFalling = true;
	FallTime = 0.0f;  // 落下時間をリセット
}