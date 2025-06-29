// ------------------------------------------------
// ActorBase.cpp
// �L�����N�^�[�̊�{�N���X
// �쐬�ҁF���N�F�A���ᩗ�
// �쐬�����F2024-11-18
// ------------------------------------------------

#include "ActorBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// �@�R���X�g���N�^
AActorBase::AActorBase(const FObjectInitializer &ObjectInitializer)
{
}

// �X�^�[�g�֐�
void AActorBase::BeginPlay()
{
	// �e�N���X��BeginPlay()
	Super::BeginPlay();

	// ������
	Init();

	// ���G���Ԃ�ݒ�
	mDamageCD = 2.0f;
}

// ���t���[���X�V
void AActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Update();

	if (mDamageTimer > 0)
	{
		mDamageTimer = UKismetMathLibrary::FClamp(mDamageTimer - GetWorld()->GetDeltaSeconds(), 0.0f, mDamageCD);
	}
}

// ������
void AActorBase::Init()
{
}

// ���S����֐�
bool AActorBase::Dead()
{
	return false;
}

// HP���炷�֐�
void AActorBase::FluctuationHP(float decrease)
{
	// ���G���Ԓ��ł���Ώ������Ȃ�
	if (mDamageTimer > 0.0f)
	{
		return;
	}

	// HP�����炷
	mHitPoint -= decrease;
	// ���G���Ԃ��J�E���g
	mDamageTimer = mDamageCD;

	// �f�o�b�O�p
	UE_LOG(LogTemp, Log, TEXT("OBJECT HP IS %d"), mHitPoint);
}
