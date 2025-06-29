// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


// 追跡状態
enum class ChaseStatus {
	None,	// 無
	Idle,	// 待機
	Chase,	// 追跡中
	TargetLost,	// 見失った状態
};

// ランダム回転に必要な情報
struct RandomRotationInfo {
	// 現在の回転角度
	float mCurrentLookAroundYaw = 0.0f;

	// 目標とする回転角度
	float mTargetLookAroundYaw = 30.0f;

	// 回転方向（1: 時計回り, -1: 反時計回り）
	int32 mLookAroundDirection = 1;

	// 回転が次に切り替わるまでのタイマー
	float mLookAroundChangeTimer = 0.0f;

	// 次の方向切り替えまでの間隔
	float mLookAroundChangeInterval = 1.5f; // 秒
};

// ランダム移動に必要な情報
struct RandomMoveInfo {
	FVector RandomPos;	// ランダム座標用
	FVector PreviousPos;	// 前回の座標
	bool IsEndMove;	// 移動を完了したか
	float MoveRange;	// 移動範囲
	float MinDistance;	// 前回の目標地点との最小距離
	float ElapsedTime;	// 経過時間
};

// 視界情報
struct VisibilityInfo
{
	float Centerangle;	// 中心角
	float Radius;	// 半径

	float GetArcLength()
	{
		return Radius * 2 * 3.14f * (Centerangle / 360.0f);
	}
};

struct RandomRotateInfo {
	float ElapsedTime;	// 経過時間
	float DeltaTime;	// 1フレーム速度
	float RotateTime;	// 回転時間
};