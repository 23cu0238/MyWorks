// Fill out your copyright notice in the Description page of Project Settings.

#include "VendingMachine.h"
#include "RecoveryHp.h" // �X�|�[������A�N�^�[�N���X(HP)
#include "RecoveryMp.h" // �X�|�[������A�N�^�[�N���X(MP)
#include "RecoveryItem.h"
#include "PlayerCharacter.h" // �v���C���[�֘A�̃w�b�_�[���C���N���[�h

// Sets default values
AVendingMachine::AVendingMachine()

{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ���b�V���R���|�[�l���g(StatickMesh)����
	mVendingMachineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VendingMachineMesh"));
	mVendingMachineMesh->SetupAttachment(RootComponent);

	// HP�̏����l��ݒ�
	mHealth = 100.0f; // �����̔��@��HP��100�ɐݒ�
}

// Called when the game starts or when spawned
void AVendingMachine::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;

	// �d�͂Ɋւ��鏉���l�ݒ�
	Velocity = FVector::ZeroVector;
	GravityForce = -980.0f;

	// ���ɂ����ɒu���Ă���
	SpawnActors();
}

// ���t���[���̌Ăяo��
void AVendingMachine::Tick(float DeltaTime)
{
	// �d�͂𑬓x�ɉ��Z
	Velocity.Z += GravityForce * DeltaTime;

	// �A�N�^�[�̈ʒu���X�V
	AddActorWorldOffset(Velocity * DeltaTime, true);

	// �Փˌ��o�̂��߂̏���
	FHitResult Hit;

	AddActorWorldOffset(Velocity * DeltaTime, true, &Hit);
	if (Hit.IsValidBlockingHit())
	{
		// �Փ˂��Ă�����Z�����̑��x�����Z�b�g
		Velocity.Z = 0.0f;
	}

	switch (mStatus)
	{
		// �n��
	case ITEM_STATUS_NONE:
	{
		// �ُ�Ȃ�
		UE_LOG(LogTemp, Log, TEXT("NONE"));
	}
	break;
	// �d�͂�������ꂽ���(�g�p���Ȃ��\����)
	case ITEM_STATUS_ADDGRAVITY:
	{
	}
	break;
	// ���g��j���Ԃֈڍs����
	case ITEM_STATUS_DESTROY_TRANSITION:
	{
		// �A�C�e���𗎂Ƃ�
		SpawnActors();
		UE_LOG(LogTemp, Log, TEXT("TRANSITION"));
		// mStatus = ITEM_STATUS_DESTROY;		//�j���Ԃ�
	}
	break;
	// ���g��j���Ԃ�
	case ITEM_STATUS_DESTROY:
	{
		// �����𕲁X�ɂ���(���͏���)
		this->Destroy();

		// �ʏ��Ԃ�
		mStatus = ITEM_STATUS_NONE;
	}
	break;
		// �I���
	}
}

void AVendingMachine::SpawnActors()
{
	// �����̍��W���擾
	FVector MyPosition = GetActorLocation();

	// �X�|�[���ʒu���������炵�Đݒ�
	FVector Offset1(100.f, 0.f, 0.f); // X������100���j�b�g
	FVector Offset2(0.f, 100.f, 0.f); // Y������100���j�b�g

	FVector SpawnPosition1 = MyPosition + Offset1;
	FVector SpawnPosition2 = MyPosition + Offset2;

	// ���[���h�̎擾
	UWorld *World = GetWorld();
	if (World)
	{
		// RecoveryHp���X�|�[�����ă��b�V����ݒ�
		ARecoveryHp *RecoveryHp = World->SpawnActor<ARecoveryHp>(ARecoveryHp::StaticClass(), SpawnPosition1, FRotator::ZeroRotator);
		if (RecoveryHp && RecoveryHp->Mesh1)
		{
			RecoveryHp->SetMesh(RecoveryHp->Mesh1);
		}

		else
		{
			UE_LOG(LogTemp, Error, TEXT("RecoveryHp or Mesh1 is NULL"));
		}

		// RecoveryMp���X�|�[�����ă��b�V����ݒ�
		ARecoveryMp *RecoveryMp = World->SpawnActor<ARecoveryMp>(ARecoveryMp::StaticClass(), SpawnPosition2, FRotator::ZeroRotator);
		if (RecoveryMp && RecoveryMp->Mesh2)
		{
			RecoveryMp->SetMesh(RecoveryMp->Mesh2);
		}

		else
		{
			UE_LOG(LogTemp, Error, TEXT("RecoveryMp or Mesh2 is NULL"));
		}
	}
}

// HP������������֐�
void AVendingMachine::FluctuationHP(float Decrease)
{
	// HP������
	mHealth -= Decrease;
	/*UE_LOG(LogTemp, Log, TEXT("Vending Machine HP: %f"), Health);*/

	// HP���[���ȉ��̏ꍇ�A�j���Ԃֈڍs
	if (mHealth <= 0)
	{
		mStatus = ITEM_STATUS_DESTROY_TRANSITION; // ��Ԃ�j��ڍs��Ԃɐݒ�
		UE_LOG(LogTemp, Warning, TEXT("Vending Machine is Destroyed"));
	}
}

bool AVendingMachine::Dead()
{
	return mHealth <= 0; // HP��0�ȉ��Ȃ�true��Ԃ�
}