// ------------------------------------------------
// GravityCharacterMovementComp.cpp
// 重力キャラクターのムーブメントコンポーネントクラス
// 作成者：李朗曦
// 作成日時：2024-12-01
// ------------------------------------------------

#include "GravityCharacterMovementComp.h"
#include "GameFramework/Character.h"

class ACharacter;

void UGravityCharacterMovementComp::PhysicsRotation(float DeltaTime)
{
	// UEが用意したもの
	// ここから
	if (!(bOrientRotationToMovement || bUseControllerDesiredRotation || bRemainVertical))
	{
		return;
	}

	if (!HasValidData() || (!CharacterOwner->Controller && !bRunPhysicsWithNoController))
	{
		return;
	}

	FRotator CurrentRotation = UpdatedComponent->GetComponentRotation(); // Normalized
	CurrentRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): CurrentRotation"));

	FRotator DeltaRot = GetDeltaRotation(DeltaTime);
	DeltaRot.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): GetDeltaRotation"));

	FRotator DesiredRotation = CurrentRotation;
	if (bOrientRotationToMovement)
	{
		DesiredRotation = ComputeOrientToMovementRotation(CurrentRotation, DeltaTime, DeltaRot);
	}
	else if (CharacterOwner->Controller && bUseControllerDesiredRotation)
	{
		DesiredRotation = CharacterOwner->Controller->GetDesiredRotation();
	}
	else if (!CharacterOwner->Controller && bRunPhysicsWithNoController && bUseControllerDesiredRotation)
	{
		if (AController *ControllerOwner = Cast<AController>(CharacterOwner->GetOwner()))
		{
			DesiredRotation = ControllerOwner->GetDesiredRotation();
		}
	}
	else
	{
		return;
	}

	const bool bWantsToBeVertical = ShouldRemainVertical();

	if (bWantsToBeVertical)
	{
		if (HasCustomGravity())
		{
			FRotator GravityRelativeDesiredRotation = (GetGravityToWorldTransform() * DesiredRotation.Quaternion()).Rotator();
			GravityRelativeDesiredRotation.Pitch = 0.f;
			GravityRelativeDesiredRotation.Yaw = FRotator::NormalizeAxis(GravityRelativeDesiredRotation.Yaw);
			GravityRelativeDesiredRotation.Roll = 0.f;
			DesiredRotation = (GetWorldToGravityTransform() * GravityRelativeDesiredRotation.Quaternion()).Rotator();
		}
		else
		{
			DesiredRotation.Pitch = 0.f;
			DesiredRotation.Yaw = FRotator::NormalizeAxis(DesiredRotation.Yaw);
			DesiredRotation.Roll = 0.f;
		}
	}
	else
	{
		DesiredRotation.Normalize();
	}

	// Accumulate a desired new rotation.
	const float AngleTolerance = 1e-3f;
	// ここまで

	// 変更ある部分
	if (!CurrentRotation.Equals(DesiredRotation, AngleTolerance))
	{
		// もし垂直になることを妨げられるのであれば、ヨー以外の回転レートをオーバーライドして、キャラクターが直立できるようにする
		if (DeltaRot.Roll == DeltaRot.Yaw && DeltaRot.Yaw == DeltaRot.Pitch)
		{
			// 2つの回転子間の球面補間を計算する
			const FQuat CurrentQuat(CurrentRotation);
			const FQuat DesiredQuat(DesiredRotation);

			// 四元数間の最短角度を得る
			const float Angle = FMath::Acos(FMath::Abs(CurrentQuat | DesiredQuat)) * 2.0f;

			// 補間率を計算する
			const float Alpha = FMath::Min(FMath::DegreesToRadians(DeltaRot.Yaw) / Angle, 1.0f);
			DesiredRotation = (Alpha == 1.0f) ? DesiredRotation : FQuat::Slerp(CurrentQuat, DesiredQuat, Alpha).Rotator();

			mDeltaRot = DeltaRot;
		}
		else
		{

			// ピッチ
			if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, DesiredRotation.Pitch, AngleTolerance))
			{
				DesiredRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, DesiredRotation.Pitch, DeltaRot.Pitch);
			}

			// ヨー
			if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, AngleTolerance))
			{
				DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRot.Yaw);
			}

			// ロール
			if (!FMath::IsNearlyEqual(CurrentRotation.Roll, DesiredRotation.Roll, AngleTolerance))
			{
				DesiredRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, DesiredRotation.Roll, DeltaRot.Roll);
			}
		}

		// 新しい回転を設定する
		DesiredRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): DesiredRotation"));
		MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, /*bSweep*/ false);
	}
}

// スタート関数
void UGravityCharacterMovementComp::BeginPlay()
{
	Super::BeginPlay();

	// 初期回転率を記録
	mOriginalRotationRate = RotationRate;
}

// 毎フレーム更新
void UGravityCharacterMovementComp::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// RotationRate = mOriginalRotationRate + GetGravityToWorldTransform;
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
