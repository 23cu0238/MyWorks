// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RecoveryItem.generated.h"

UCLASS()
class ANTIGRAVITY_API ARecoveryItem : public AActor
{
	GENERATED_BODY()
	
public:	
	//コンストラクタ
	ARecoveryItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//毎フレームの更新処理
	virtual void Tick(float DeltaTime) override;

protected:
	//アイテムを回転
	virtual void RotateItem(float rotateSpeed);

	//アイテムを傾ける
	virtual void TiltItem(float tilt);

	//アイテムの上下運動
	virtual void UpdownAmplitude();

	//重力の設定
	virtual void GravityScale(float DeltaTime);

	//重力初期化関数
	virtual void GravityInitialize();

	//アイテムを出すかを判断する関数
	//virtual bool discharge();

	//エディター上で編集できるメンバー変数
public:
	//アイテム接触用コリジョン
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* mpBoxComp;

	//アイテムメッシュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* mpItemMesh;

	//Hp回復アイテム
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* Mesh1;

	//Mp回復アイテム
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* Mesh2;

	//メッシュを設定する関数
	UFUNCTION(BlueprintCallable, Category = "Mesh Control")
	void SetMesh(UStaticMesh* NewMesh);
	
protected:
	//ベースコンポーネント
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* mpBase;

	//回転速度
	UPROPERTY(EditAnywhere)
	float mItemRotationSpeed;	//回転スピード

	UPROPERTY(EditAnywhere)
	float mUpDownSpeed;			//上下するスピード

	UPROPERTY(EditAnywhere)
	float mAmplitude;			//振幅(上下の範囲)

	UPROPERTY(EditAnywhere)
	float mBaseHeight;			//基準の高さ

	UPROPERTY(EditAnywhere)
	float mTilt;				//傾き

	UPROPERTY(EditAnywhere)
	float mItemAddHP;			//HP＋

	UPROPERTY(EditAnywhere)
	float mItemAddMP;			//MP＋

	UPROPERTY(EditAnywhere)
	bool isHit;

	UFUNCTION(BlueprintCallable)
	bool IsHit() { return isHit; }

private:
	FVector Velocity;
	float GravityForce;

protected:
	UFUNCTION(BlueprintCallable)
	virtual void OnOverlapRecovery(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual void OnOverlapRecoveryEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
