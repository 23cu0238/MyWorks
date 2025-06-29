// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundTypeEnemy.h"
#include "AttackBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FlightTypeAttack.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PlayerAttack.h"
#include "PlayerCharacter.h"
#include "CollisionQueryParams.h"
#include "Components/BoxComponent.h"
#include "Components/LineBatchComponent.h"
#include "Components/CapsuleComponent.h"
#include "ShootingAttack.h"

// 
// �N���X���FGround
// �p�r�F��s�^�C�v�̓G�̎���
// �쐬�ҁF23cu0216���n����
// 

AGroundTypeEnemy::AGroundTypeEnemy()
{
    // �U���͈͐ݒ�
    UBoxComponent* attackboxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackComp"));
    attackboxcomp->SetupAttachment(RootComponent);
    mAttackComp = attackboxcomp;

    // Overlap�C�x���g�̓o�^
    mAttackComp->OnComponentBeginOverlap.AddDynamic(this, &AGroundTypeEnemy::OnOverlapAttack);
    mAttackComp->OnComponentEndOverlap.AddDynamic(this, &AGroundTypeEnemy::OnOverlapAttackEnd);
}

AGroundTypeEnemy::~AGroundTypeEnemy()
{
}

// �������Ȃ�
void AGroundTypeEnemy::BeginPlay()
{
    Super::BeginPlay();

    // �����ʒu�ݒ�
    mInitPos = GetActorLocation();

    // �U���͈͂̐ݒ�
    mAttackComp->SetActive(true);

    // Overlap�C�x���g�̓o�^
    mAttackComp->OnComponentBeginOverlap.AddDynamic(this, &AGroundTypeEnemy::OnOverlapAttack);
    mAttackComp->OnComponentEndOverlap.AddDynamic(this, &AGroundTypeEnemy::OnOverlapAttackEnd);

    mShootiongAttack = NewObject<UShootingAttack>();
    mShootiongAttack->Init(this,mShootingRadius,GetWorld(),mShootingName);
}

// �X�V
void AGroundTypeEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //PlayAttack();
}

void AGroundTypeEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
}

// �ړ��X�V����
void AGroundTypeEnemy::UpdateMove(float DeltaTime)
{
}

//
// ��������
//

// �U������
bool AGroundTypeEnemy::PlayAttack()
{
    // �G�������
    if (!SearchPlayer())
    {
        mShootingTime = 0.0f;
        return false;
    }
    // 
    if (!mOverlapOtherActor)
        return false;

    mShootingTime += GetWorld()->GetDeltaSeconds();

    if (mShootingTime <= mShootingCoolTime)
        return false;

    // 
    FVector ShootingPos = GetActorLocation() + (GetActorForwardVector() + 2.0f);
    mShootiongAttack->Attack(Power, mOverlapOtherActor, ShootingPos);
    mShootingTime = 0.0f;
    return true;
}

void AGroundTypeEnemy::OnOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("Player"))
    {
        UE_LOG(LogTemp, Log, TEXT("Hit Player, Comp name:%s"), *OtherComp->GetName());
        bIsTargetOverlappingAttack = true;
        mOverlapOtherActor = OtherActor;
        mOverlapOtherComp = OtherComp;
    }
}

void AGroundTypeEnemy::OnOverlapAttackEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor->ActorHasTag("Player"))
    {
        UE_LOG(LogTemp, Log, TEXT("Hit Player End"));
        bIsTargetOverlappingAttack = false;
        mOverlapOtherActor = nullptr;
        mOverlapOtherComp = nullptr;
    }
}

void AGroundTypeEnemy::AttackFunction(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
    if (!OtherActor || !OtherComp)
    {
        return;
    }

    if (OtherActor->ActorHasTag("Player"))
    {
        TObjectPtr<UConventionalAttack> AttackObj = NewObject<UConventionalAttack>();
        AttackObj->Attack(Power, OtherActor);
    }
}