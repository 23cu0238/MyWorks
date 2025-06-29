//�����������������̂�
//�L�����N�^�[�Ɏ�������Ƃ��͎Q�l�ɂ��Ă�������

#include "TESTPAWN.h"
#include"ShootingAttack.h"

// Sets default values
ATESTPAWN::ATESTPAWN()
{
	//UE�ŗL
	PrimaryActorTick.bCanEverTick = true;
}

// ������
void ATESTPAWN::BeginPlay()
{
	//�U���̎��
	FString normalName = "Normal";
	//�U���̎��
	FString missileName = "Missile";
	//UE�ŗL
	Super::BeginPlay();
	//�U����K�����������邱��
	shoot = NewObject<UShootingAttack>();
	missile = NewObject<UShootingAttack>();
	//����͐�΂ɌĂԂ���
	shoot->Init(this, radius, GetWorld(), normalName);
	missile->Init(this, radius, GetWorld(), missileName);
	//���Ԃ�ݒ�(�ŏ�����ł�������Α喃��0�ɂ��Ă�������)
	timer = time;
}

//���s
void ATESTPAWN::Tick(float DeltaTime)
{
	//UE�ŗL
	Super::Tick(DeltaTime);
	//�ˌ�
	Shoot();
	//ShootMissile();
}

//�悭�킩���
void ATESTPAWN::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
//�ˌ�
void ATESTPAWN::Shoot()
{
	//���Ԃ���Z
	timer -= GetWorld()->DeltaTimeSeconds;
	if (timer < 0)
		timer = 0;

	//�ݒ肳�ꂽ���Ԃ�҂��Ă��Ȃ���ΏI��
	if (timer > 0)
		return;

	//�Ώۂ܂��͂���ɕt������R���|�[�l���g����̏ꍇ�I��
	//�������d���Ȃ邽��try�͂����A���O�����o��
	if (mTargetActor == nullptr || mTargetComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("TARGET IS NOT SETTING"))
			//���Ԃ�߂�
			timer = time;
		return;
	}
	//�U���������Ƃ���O�Ɏc��
	UE_LOG(LogTemp, Log, TEXT("FIRE"));
	//�R���|�[�l���g�Ɏˌ��𖽗�
	shoot->Attack(0.0f, mTargetActor, GetActorForwardVector());
	//���[�邽���ނ���������Ă�
	timer = time;
}
//�ˌ��֐�
void ATESTPAWN::ShootMissile()
{
	////���Ԃ���Z
	//timer -= GetWorld()->DeltaTimeSeconds;
	//if (timer < 0)
	//	timer = 0;

	////�ݒ肳�ꂽ���Ԃ�҂��Ă��Ȃ���ΏI��
	//if (timer > 0)
	//	return;

	////�Ώۂ܂��͂���ɕt������R���|�[�l���g����̏ꍇ�I��
	////�������d���Ȃ邽��try�͂����A���O�����o��
	//if (mTargetActor == nullptr || mTargetComp == nullptr)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("TARGET IS NOT SETTING"))
	//		//���Ԃ�߂�
	//		timer = time;
	//	return;
	//}
	////�U���������Ƃ���O�Ɏc��
	//UE_LOG(LogTemp, Log, TEXT("FIRE"));
	////�R���|�[�l���g�Ɏˌ��𖽗�
	//->Attack(0.0f, mTargetActor, mTargetComp);
	////���[�邽���ނ���������Ă�
	//timer = time;
}

void ATESTPAWN::OnHit(UPrimitiveComponent* HitCom, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		UE_LOG(LogTemp, Log, TEXT("DETECTION TARGET"));
		mTargetActor = OtherActor;
		mTargetComp = OtherComp;
	}
}