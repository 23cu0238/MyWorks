// ------------------------------------------------
// GravityController.h
// コントローラクラス
// 作成者：李朗曦
// 作成日時：2024-11-28
// ------------------------------------------------

#include "GravityController.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// 回転の更新
void AGravityController::UpdateRotation(float DeltaTime)
{
	// 必要な変数を取得
	FVector GravityDirection = FVector::DownVector;
	if (ACharacter *PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		if (UCharacterMovementComponent *MoveComp = PlayerCharacter->GetCharacterMovement())
		{
			GravityDirection = MoveComp->GetGravityDirection();
		}
	}

	// ワールド空間で現在のコントロール回転を取得する
	FRotator ViewRotation = GetControlRotation();

	// 重力の変化による回転があれば、それを追加します
	// カメラに重力の回転を自動的に補正させたくない場合は、このコードブロックを削除します
	if (!LastFrameGravity.Equals(FVector::ZeroVector))
	{
		FVector LerpVector = UKismetMathLibrary::VLerp(LastFrameGravity, GravityDirection, 0.2);
		const FQuat DeltaGravityRotation = FQuat::FindBetweenNormals(LastFrameGravity, LerpVector);
		const FQuat WarpedCameraRotation = DeltaGravityRotation * FQuat(ViewRotation);

		ViewRotation = WarpedCameraRotation.Rotator();
	}

	LastFrameGravity = GravityDirection;

	// ビューの回転をワールド空間から重力相対空間に変換します
	// これで、重力の影響がないかのように回転を扱うことができます
	ViewRotation = GetGravityRelativeRotation(ViewRotation, GravityDirection);

	// ViewRotationに適用されるデルタを計算する
	FRotator DeltaRot(RotationInput);

	if (PlayerCameraManager)
	{
		ACharacter *PlayerCharacter = Cast<ACharacter>(GetPawn());

		PlayerCameraManager->ProcessViewRotation(DeltaTime, ViewRotation, DeltaRot);

		// 重力に対して常に水平になるように、カメラのロールをゼロにする
		ViewRotation.Roll = 0;

		// 回転をワールド空間に戻し、現在のコントロール回転として設定する
		SetControlRotation(GetGravityWorldRotation(ViewRotation, GravityDirection));

		ViewRotation = GetControlRotation();
	}

	// プレイヤーキャラクターの向きを変更
	APawn *const P = GetPawnOrSpectator();
	if (P)
	{
		P->FaceRotation(ViewRotation, DeltaTime);
	}
}

// 回転を世界空間から重力相対空間に変換する
FRotator AGravityController::GetGravityRelativeRotation(FRotator Rotation, FVector GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		FQuat GravityRotation = FQuat::FindBetweenNormals(GravityDirection, FVector::DownVector);
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}

	return Rotation;
}

// 重力相対空間から世界空間に回転を変換します
FRotator AGravityController::GetGravityWorldRotation(FRotator Rotation, FVector GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		FQuat GravityRotation = FQuat::FindBetweenNormals(FVector::DownVector, GravityDirection);
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}

	return Rotation;
}