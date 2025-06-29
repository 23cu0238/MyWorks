// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MissileState.h"
#include "Components/ActorComponent.h"
#include "InductionState.generated.h"

class ConventionalAttack;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ANTIGRAVITY_API UInductionState : public UActorComponent, public IMissileState
{
    GENERATED_BODY()

public:
    //�R���X�g���N�^
    UInductionState();

public:
    //��Ԃ̏�����
    //������ : �I�[�i�[�ɂȂ�N���X
    void OnEnter(UMissile *owner) override;
    //���s
    //������ : �I�[�i�[�ɂȂ�N���X
    void OnUpdate(UMissile *owner) override;
    //��Ԃ̏I��
    //������ : �I�[�i�[�ɂȂ�N���X
    void OnExit(UMissile *owner) override;
    //�G�ꂽ���̏���
    //������ : �I�[�i�[�ɂȂ�N���X
    //������ : �G�ꂽ�A�N�^�[
    //��O���� : �G�ꂽ�A�N�^�[�̃v���~�e�B�u�R���|�[�l���g
    //��l���� : �G�ꂽ�A�N�^�[�̏��
    void OnHit(UMissile *owner, AActor *HitActor, UPrimitiveComponent HitComp, const FHitResult &Hit);

private:
    //��������`���悤�Ɉړ�������
    void Induction();
    //�ړI�n�t�߂ɂ��邩
    bool IsReachedEnd();
    //�ړ���v�Z����
    FVector Evaluate(float t);

private:
    int count = 0;
    float timer = 0;
    float mHitstopTimer = 0.0f;

    //TObjectPtr<UConventionalAttack> mAttack;

    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    FVector mAnchorpoint;

    FVector startPosition;

    FVector Start;

    UMissile *mOwner;
};
