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

	//�f�t�H���g�l�̐ݒ�
	FallTime = 0.0f;
	FallThreshold = 0.2f;  // 0.2�b�ȏ㗎��������_���[�W
	DamageAmount = 50.0f;   // �_���[�W��
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
		FallTime += DeltaTime;  // �������Ă��鎞�Ԃ����Z
		if (FallTime >= FallThreshold)
		{
			ApplyFallDamage();  // �_���[�W��^����
			bIsFalling = false;  // 1�񂾂��_���[�W��^����悤�ɂ���
		}
	}
}

// �������Ԃ��o�߂����ꍇ�Ƀ_���[�W��^����
void AFallDamageObject::ApplyFallDamage()
{
    // ���[���h���̑S�ẴA�N�^�[�ƏՓ˂��m�F���邽�߂̃X�t�B�A�L���X�g
    FVector ActorLocation = GetActorLocation(); // ���̃A�N�^�[�̌��݂̈ʒu
    float Radius = 200.0f; // �_���[�W��^����͈͂̔��a

    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); // �������g�𖳎�

    // �X�t�B�A�̏Փ˔���
    bool bOverlap = GetWorld()->OverlapMultiByObjectType(
        OverlapResults,
        ActorLocation,
        FQuat::Identity,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn), // Pawn �`�����l�����w��i�G�͒ʏ킱�̃`�����l���j
        FCollisionShape::MakeSphere(Radius),
        QueryParams
    );

    if (bOverlap)
    {
        for (const FOverlapResult& Result : OverlapResults)
        {
            AActor* HitActor = Result.GetActor(); // �Փ˂����A�N�^�[���擾

            if (HitActor && HitActor->ActorHasTag(TEXT("Enemy"))) // Enemy �^�O�������A�N�^�[�𔻒�
            {
                // �_���[�W��K�p
                UGameplayStatics::ApplyDamage(
                    HitActor,                   // �_���[�W��^����Ώ�
                    DamageAmount,               // �_���[�W��
                    GetInstigatorController(),  // ���̃_���[�W��^�����R���g���[���[
                    this,                       // �_���[�W�̌����ƂȂ����A�N�^�[
                    nullptr                     // �_���[�W�^�C�v�i�f�t�H���g�� null�j
                );

                UE_LOG(LogTemp, Warning, TEXT("Applied %f damage to %s"), DamageAmount, *HitActor->GetName());
            }
        }
    }
	UE_LOG(LogTemp, Warning, TEXT("Fall damage applied: %f"), DamageAmount);
}

// ���������ǂ�����ݒ�
void AFallDamageObject::StartFalling()
{
	bIsFalling = true;
	FallTime = 0.0f;  // �������Ԃ����Z�b�g
}