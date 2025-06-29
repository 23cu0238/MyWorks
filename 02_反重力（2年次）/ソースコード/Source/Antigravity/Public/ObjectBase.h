// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ANTIGRAVITY_API ObjectBase
{
public:
	//�R���X�g���N�^
	ObjectBase();


protected:
	//�I�[�o�[���b�v�ڐG���n�߂��Ƃ��ɌĂ΂��C�x���g�֐�
	UFUNCTION()
	//virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32* OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	UPROPERTY()
	float mDropDownTime;		//��������

	UPROPERTY()
	float mMass;				//����
	UPROPERTY()
	float mSpeed;				//�X�s�[�h

	//�����̔��@�̏��
	enum VendingMachine_STATUS
	{
		ITEM_STATUS_NONE,				//�ω��Ȃ�
		ITEM_STATUS_ADDGRAVITY,			//�d�͂�t�^���ꂽ���
		ITEM_STATUS_DROPDOWN,			//�������
		ITEM_STATUS_DESTROY_TRANSITION,	//�j���ԂɈڍs�J�n
		ITEM_STATUS_DESTROY				//�����̔��@�����S�j��
	};

	VendingMachine_STATUS mStatus;		//�X�e�[�^�X�Ǘ�
	UPROPERTY(EditAnywhere, Category = "VendingMachine") float HP;
};
