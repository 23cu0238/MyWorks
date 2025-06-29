// ------------------------------------------------
// GravityCharacterMovementComp.cpp
// �d�̓L�����N�^�[�̃��[�u�����g�R���|�[�l���g�N���X
// �쐬�ҁF���N�F
// �쐬�����F2024-12-01
// ------------------------------------------------

#include "GravityCharacterMovementComp.h"
#include "GameFramework/Character.h"

class ACharacter;

void UGravityCharacterMovementComp::PhysicsRotation(float DeltaTime)
{
	// UE���p�ӂ�������
	// ��������
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
	// �����܂�

	// �ύX���镔��
	if (!CurrentRotation.Equals(DesiredRotation, AngleTolerance))
	{
		// ���������ɂȂ邱�Ƃ�W������̂ł���΁A���[�ȊO�̉�]���[�g���I�[�o�[���C�h���āA�L�����N�^�[�������ł���悤�ɂ���
		if (DeltaRot.Roll == DeltaRot.Yaw && DeltaRot.Yaw == DeltaRot.Pitch)
		{
			// 2�̉�]�q�Ԃ̋��ʕ�Ԃ��v�Z����
			const FQuat CurrentQuat(CurrentRotation);
			const FQuat DesiredQuat(DesiredRotation);

			// �l�����Ԃ̍ŒZ�p�x�𓾂�
			const float Angle = FMath::Acos(FMath::Abs(CurrentQuat | DesiredQuat)) * 2.0f;

			// ��ԗ����v�Z����
			const float Alpha = FMath::Min(FMath::DegreesToRadians(DeltaRot.Yaw) / Angle, 1.0f);
			DesiredRotation = (Alpha == 1.0f) ? DesiredRotation : FQuat::Slerp(CurrentQuat, DesiredQuat, Alpha).Rotator();

			mDeltaRot = DeltaRot;
		}
		else
		{

			// �s�b�`
			if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, DesiredRotation.Pitch, AngleTolerance))
			{
				DesiredRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, DesiredRotation.Pitch, DeltaRot.Pitch);
			}

			// ���[
			if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, AngleTolerance))
			{
				DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRot.Yaw);
			}

			// ���[��
			if (!FMath::IsNearlyEqual(CurrentRotation.Roll, DesiredRotation.Roll, AngleTolerance))
			{
				DesiredRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, DesiredRotation.Roll, DeltaRot.Roll);
			}
		}

		// �V������]��ݒ肷��
		DesiredRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): DesiredRotation"));
		MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, /*bSweep*/ false);
	}
}

// �X�^�[�g�֐�
void UGravityCharacterMovementComp::BeginPlay()
{
	Super::BeginPlay();

	// ������]�����L�^
	mOriginalRotationRate = RotationRate;
}

// ���t���[���X�V
void UGravityCharacterMovementComp::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// RotationRate = mOriginalRotationRate + GetGravityToWorldTransform;
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
