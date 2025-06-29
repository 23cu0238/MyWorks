// ���˂����e�̑S�Ă��i�[����N���X
// static�N���X
// �Ȃ����I�u�W�F�N�g�Ƃ��Ĕz�u���Ȃ���΂Ȃ�Ȃ��̂�
// �ˌ�����ۂ̓��x���ɔz�u���Ă�������
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.h"
#include "DefaultBullet.h"
#include "Shootable.h"
#include "BulletContainer.generated.h"

UCLASS()
class ANTIGRAVITY_API ABulletContainer : public AActor
{
	// UE�ŗL
	GENERATED_BODY()

public:
	// �V���O���g���I�u�W�F�N�g���擾���邽�� static ���\�b�h�ɂ���
	static ABulletContainer *GetInstance();
	// �e��ݒ肷��֐�
	// ����1 : �e�̖��O�E���
	// ����2 : �e���i�[����Ώۂ̃A�h���X
	void GetBulletClass(FString tagName, TSubclassOf<class ABullet> &bullet);
	// �e�̏�Ԃ�ݒ肷��N���X
	// ����1 : �e�̖��O�E���
	IShootable *SetupState(FString tagName);

private:
	// �������֐�
	virtual void BeginPlay() override;

private:
	// �R���X�g���N�^
	ABulletContainer();

private:
	// �C���X�^���X�� private static �ŕێ�����
	static ABulletContainer *Instance;

private:
	// �e�Ɩ��O�����т���e��
	// �u���[�v�����g�œo�^���Ă�������
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TMap<FString, TSubclassOf<class ABullet>> bulletMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<FString, TSubclassOf<UObject>> StatesMap;
};
