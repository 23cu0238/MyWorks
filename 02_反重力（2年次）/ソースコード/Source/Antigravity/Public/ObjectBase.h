// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ANTIGRAVITY_API ObjectBase
{
public:
	//コンストラクタ
	ObjectBase();


protected:
	//オーバーラップ接触し始めたときに呼ばれるイベント関数
	UFUNCTION()
	//virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32* OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	UPROPERTY()
	float mDropDownTime;		//落下時間

	UPROPERTY()
	float mMass;				//質量
	UPROPERTY()
	float mSpeed;				//スピード

	//自動販売機の状態
	enum VendingMachine_STATUS
	{
		ITEM_STATUS_NONE,				//変化なし
		ITEM_STATUS_ADDGRAVITY,			//重力を付与された状態
		ITEM_STATUS_DROPDOWN,			//落下状態
		ITEM_STATUS_DESTROY_TRANSITION,	//破壊状態に移行開始
		ITEM_STATUS_DESTROY				//自動販売機を完全破壊
	};

	VendingMachine_STATUS mStatus;		//ステータス管理
	UPROPERTY(EditAnywhere, Category = "VendingMachine") float HP;
};
