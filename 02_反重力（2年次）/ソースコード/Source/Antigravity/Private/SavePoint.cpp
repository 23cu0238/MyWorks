// Fill out your copyright notice in the Description page of Project Settings.


#include "SavePoint.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

//�ÓI�����o�[�ϐ��̒�`
FVector
ASavePoint::RespawnLocation = FVector::ZeroVector;

// Sets default values
ASavePoint::ASavePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // �R���W�����{�b�N�X�̃Z�b�g�A�b�v
    UBoxComponent* CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    //�R���W�����{�b�N�X�̃T�C�Y���g��
//<<<<<<< HEAD
    CollisionBox->SetBoxExtent(FVector(300.0f, 300.0f, 300.0f));    //�͈͒���
//=======
    CollisionBox->SetBoxExtent(FVector(300.0f, 300.0f, 300.0f));    //�͈͂��L������
//>>>>>>> f406533f227dcf8ab0fba6555e2bef168b536ab2

    // �R���W�����C�x���g���o�C���h
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASavePoint::OnOverlapBegin);

    bIsActive = false; // ������Ԃ͔�A�N�e�B�u
}

// Called when the game starts or when spawned
void ASavePoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASavePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASavePoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("Player"))
    {
        // ���̃Z�[�u�|�C���g�����Z�b�g
        for (TActorIterator<ASavePoint> It(GetWorld()); It; ++It)
        {
            It->bIsActive = false;
        }

        // ���̃Z�[�u�|�C���g���A�N�e�B�u�ɐݒ�
        bIsActive = true;

        // ���X�|�[���n�_���X�V
        RespawnLocation = GetActorLocation();
        UE_LOG(LogTemp, Log, TEXT("SavePoint Activated at: %s"), *RespawnLocation.ToString());

    }
}