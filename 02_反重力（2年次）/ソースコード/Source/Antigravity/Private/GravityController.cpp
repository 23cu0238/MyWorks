// ------------------------------------------------
// GravityController.h
// �R���g���[���N���X
// �쐬�ҁF���N�F
// �쐬�����F2024-11-28
// ------------------------------------------------

#include "GravityController.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// ��]�̍X�V
void AGravityController::UpdateRotation(float DeltaTime)
{
	// �K�v�ȕϐ����擾
	FVector GravityDirection = FVector::DownVector;
	if (ACharacter *PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		if (UCharacterMovementComponent *MoveComp = PlayerCharacter->GetCharacterMovement())
		{
			GravityDirection = MoveComp->GetGravityDirection();
		}
	}

	// ���[���h��ԂŌ��݂̃R���g���[����]���擾����
	FRotator ViewRotation = GetControlRotation();

	// �d�͂̕ω��ɂ���]������΁A�����ǉ����܂�
	// �J�����ɏd�͂̉�]�������I�ɕ␳���������Ȃ��ꍇ�́A���̃R�[�h�u���b�N���폜���܂�
	if (!LastFrameGravity.Equals(FVector::ZeroVector))
	{
		FVector LerpVector = UKismetMathLibrary::VLerp(LastFrameGravity, GravityDirection, 0.2);
		const FQuat DeltaGravityRotation = FQuat::FindBetweenNormals(LastFrameGravity, LerpVector);
		const FQuat WarpedCameraRotation = DeltaGravityRotation * FQuat(ViewRotation);

		ViewRotation = WarpedCameraRotation.Rotator();
	}

	LastFrameGravity = GravityDirection;

	// �r���[�̉�]�����[���h��Ԃ���d�͑��΋�Ԃɕϊ����܂�
	// ����ŁA�d�͂̉e�����Ȃ����̂悤�ɉ�]���������Ƃ��ł��܂�
	ViewRotation = GetGravityRelativeRotation(ViewRotation, GravityDirection);

	// ViewRotation�ɓK�p�����f���^���v�Z����
	FRotator DeltaRot(RotationInput);

	if (PlayerCameraManager)
	{
		ACharacter *PlayerCharacter = Cast<ACharacter>(GetPawn());

		PlayerCameraManager->ProcessViewRotation(DeltaTime, ViewRotation, DeltaRot);

		// �d�͂ɑ΂��ď�ɐ����ɂȂ�悤�ɁA�J�����̃��[�����[���ɂ���
		ViewRotation.Roll = 0;

		// ��]�����[���h��Ԃɖ߂��A���݂̃R���g���[����]�Ƃ��Đݒ肷��
		SetControlRotation(GetGravityWorldRotation(ViewRotation, GravityDirection));

		ViewRotation = GetControlRotation();
	}

	// �v���C���[�L�����N�^�[�̌�����ύX
	APawn *const P = GetPawnOrSpectator();
	if (P)
	{
		P->FaceRotation(ViewRotation, DeltaTime);
	}
}

// ��]�𐢊E��Ԃ���d�͑��΋�Ԃɕϊ�����
FRotator AGravityController::GetGravityRelativeRotation(FRotator Rotation, FVector GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		FQuat GravityRotation = FQuat::FindBetweenNormals(GravityDirection, FVector::DownVector);
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}

	return Rotation;
}

// �d�͑��΋�Ԃ��琢�E��Ԃɉ�]��ϊ����܂�
FRotator AGravityController::GetGravityWorldRotation(FRotator Rotation, FVector GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		FQuat GravityRotation = FQuat::FindBetweenNormals(FVector::DownVector, GravityDirection);
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}

	return Rotation;
}