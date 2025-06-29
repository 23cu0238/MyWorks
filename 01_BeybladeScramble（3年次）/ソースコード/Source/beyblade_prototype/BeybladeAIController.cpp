// BeybladeAIController.cpp

#include "BeybladeAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BeybladeBase.h" // ��œG�̏����擾����ۂɎg�p

ABeybladeAIController::ABeybladeAIController()
{
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ABeybladeAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (BehaviorTreeAsset)
    {
        // �u���b�N�{�[�h�����������A�r�w�C�r�A�c���[���J�n���܂�
        BlackboardComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
        BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);
    }
}