// Fill out your copyright notice in the Description page of Project Settings.


#include "FlightTypeEnemy.h"
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

// 
// �N���X���FFlightTypeEnemy
// �p�r�F��s�^�C�v�̓G�̎���
// �쐬�ҁF23cu0216���n����
// 

AFlightTypeEnemy::AFlightTypeEnemy()
{
	mEnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	mEnemyMesh->SetupAttachment(RootComponent);

    // �U���͈͐ݒ�
    TObjectPtr<UBoxComponent> attackboxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackComp"));
    attackboxcomp->SetupAttachment(RootComponent);
    mAttackComp = attackboxcomp;
}

AFlightTypeEnemy::~AFlightTypeEnemy()
{
}

// �������Ȃ�
void AFlightTypeEnemy::BeginPlay()
{
	Super::BeginPlay();

	// �����ʒu�ݒ�
	mInitPos = GetActorLocation();

    // �U���͈͂̐ݒ�
    mAttackComp->SetActive(true);

    // Overlap�C�x���g�̓o�^
    mAttackComp->OnComponentBeginOverlap.AddDynamic(this, &AFlightTypeEnemy::OnOverlapAttack);
    mAttackComp->OnComponentEndOverlap.AddDynamic(this, &AFlightTypeEnemy::OnOverlapAttackEnd);
}

// �X�V
void AFlightTypeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �ړ��X�V����
	UpdateMove(DeltaTime);

    // �U���X�V����
    PlayAttack();

    if (bIsTargetOverlappingAttack)
    {
        if (mOverlapOtherActor && mOverlapOtherComp)
        {
            AttackFunction(mOverlapOtherActor, mOverlapOtherComp);
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("mOverlapOtherActor,mOverlapOtherComp is null"));
        }
    }
}

// �ړ��X�V����
void AFlightTypeEnemy::UpdateMove(float DeltaTime)
{
    switch (mCStatus)
    {
    case ChaseStatus::None:
        break;
        // �ҋ@���
    case ChaseStatus::Idle:
        // ���G
        if (SearchPlayer())
        {
            //UE_LOG(LogTemp, Log, TEXT("Player detected, switching to Chase state."));
            mCStatus = ChaseStatus::Chase;
        }
        // ���Ȃ����
        else
        {
            // �����_���ړ������
            RandomMove();
            UE_LOG(LogTemp, Log, TEXT("Idle State: Executing RandomMove"));
            // �����_���ړ�������������
            if (mRandomMoveInfo.IsEndMove)
            {
                // �����_����]��s��
                RotateAndObserve();
            }
        }
        break;

    case ChaseStatus::Chase:
        if (SearchPlayer())
        {
            if (mOverlapOtherActor)
            {
                mPlayerPos = mOverlapOtherActor->GetActorLocation();
                FindAndMoveToLocation(mPlayerPos);
            }
        }
        else
        {
            //UE_LOG(LogTemp, Warning, TEXT("Player lost, switching to TargetLost state."));
            mLastKnownPosition = mPlayerPos;
            bHasLastKnownPosition = true;
            mCStatus = ChaseStatus::TargetLost;
        }
        break;

    case ChaseStatus::TargetLost:
        // ���G����݂�
        if (SearchPlayer())
        {
            //UE_LOG(LogTemp, Log, TEXT("Player reacquired, switching to Chase state."));
            mLostnowTime = 0.0f;
            mCStatus = ChaseStatus::Chase;
        }
        else
        {
            if (bHasLastKnownPosition)
            {
                float DistXY = FVector::Dist(
                    FVector(mLastKnownPosition.X, mLastKnownPosition.Y, 0),
                    FVector(GetActorLocation().X, GetActorLocation().Y, 0)
                );

                if (DistXY <= 100.0f)
                {
                    //UE_LOG(LogTemp, Log, TEXT("Reached last known position."));
                    bHasLastKnownPosition = false; // ���Z�b�g
                    mLostnowTime = 0.0f;
                }
                else
                {
                    //UE_LOG(LogTemp, Log, TEXT("Moving to last known position: %s"), *mLastKnownPosition.ToString());
                    FindAndMoveToLocation(mLastKnownPosition);
                }
            }
            else
            {
                mLostnowTime += DeltaTime;

                RotateAndObserve(); // ���͂���n��

                if (mLostnowTime > mLostTime)
                {
                    //UE_LOG(LogTemp, Log, TEXT("Target lost completely, returning to initial position."));
                    FindAndMoveToLocation(mInitPos);

                    float DistXY = FVector::Dist(
                        FVector(mInitPos.X, mInitPos.Y, 0),
                        FVector(GetActorLocation().X, GetActorLocation().Y, 0)
                    );

                    if (DistXY <= 150.0f)
                    {
                        //UE_LOG(LogTemp, Log, TEXT("Returned to initial position, switching to Idle state."));
                        mLostnowTime = 0.0f;
                        mCStatus = ChaseStatus::Idle; // Idle �ɑJ��
                    }
                }
            }
        }
        break;

    default:
        break;
    }
}

// �U������
bool AFlightTypeEnemy::PlayAttack()
{
    // �A�j���[�V����BP�̂ق��ɂČĂяo����Ă��Ȃ����
    if (!mIsActiveAttack)
    {
        // �����蔻��𖳌���
        mAttackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        mAttackComp->SetGenerateOverlapEvents(false);
        UE_LOG(LogTemp, Log, TEXT("mIsActiveAttack is false."));
        return false;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("mIsActiveAttack is true."));
        // �����蔻���L����
        mAttackComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        mAttackComp->SetGenerateOverlapEvents(true);

        // �O���ɃI�u�W�F�N�g���Ȃ����m�F��
        FVector Start = GetActorLocation();
        TArray<FHitResult> OutHits;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this);
        //FVector End = Start + (GetActorForwardVector() * mMoveSpeed * GetWorld()->GetDeltaSeconds());
        FVector End = Start + (GetActorForwardVector() * 400.0f);

        bool RayHit = GetWorld()->LineTraceMultiByObjectType(OutHits, Start, End,
            FCollisionObjectQueryParams::AllObjects, CollisionParams);

        if (RayHit && OutHits.Num() > 0)
        {
            if (OutHits[0].GetActor()->ActorHasTag("Player"))
            {
                // ����������̂�Player�^�ɃL���X�g�����
                APlayerCharacter* pPlayer;
                pPlayer = Cast<APlayerCharacter>(OutHits[0].GetActor());

                if (pPlayer != nullptr)
                {
                    // �U�����@�͓ːi�Ȃ̂�
                    // �����őO�����ɑf�����ړ���s��
                    float speed = (mMoveSpeed * GetWorld()->GetDeltaSeconds()) * 2;
                    FVector NewPos = FVector(GetActorLocation() + (GetActorForwardVector() * speed));
                    SetActorLocation(NewPos);
                }
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("Hit %s."), *OutHits[0].GetActor()->GetName());
            }
        }
        else
        {
            float speed = (mMoveSpeed * GetWorld()->GetDeltaSeconds()) * 2;
            FVector NewPos = FVector(GetActorLocation() + (GetActorForwardVector() * speed));
            SetActorLocation(NewPos);
        }
    }

    // �U���\�͈͂�m�F
    float Dist = FVector::Dist(FVector(GetActorLocation().X, GetActorLocation().Y, 0 ) 
        , FVector(mPlayerPos.X, mPlayerPos.Y, 0));
    mbCanAttack = mbCanAttackRadius > Dist;

    return true;
}

// AttackComp�ɓ����������ɌĂ΂��֐�
void AFlightTypeEnemy::OnOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("Player"))
    {
        UE_LOG(LogTemp, Log, TEXT("Hit Player, Comp name:%s"), *OtherComp->GetName());
        bIsTargetOverlappingAttack = true;
        mOverlapOtherActor = OtherActor;
        mOverlapOtherComp = OtherComp;
    }
}

// AttackComp���甲�������ɌĂ΂��֐�
void AFlightTypeEnemy::OnOverlapAttackEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor->ActorHasTag("Player"))
    {
        UE_LOG(LogTemp, Log, TEXT("Hit Player End"));
        bIsTargetOverlappingAttack = false;
        mOverlapOtherActor = nullptr;
        mOverlapOtherComp = nullptr;
    }
}

// FlightTypeEnemy�̍U������
void AFlightTypeEnemy::AttackFunction(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
    if (!OtherActor || !OtherComp)
    {
        UE_LOG(LogTemp, Log, TEXT("OverlapOtherActor,OverlapOtherComp is null"));
        return;
    }

    if (OtherActor->ActorHasTag("Player"))
    {
        TObjectPtr<UConventionalAttack> AttackObj = NewObject<UConventionalAttack>();
        AttackObj->Attack(Power, OtherActor);
    }
}