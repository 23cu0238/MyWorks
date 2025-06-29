// BeybladeAIController.cpp

#include "BeybladeAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BeybladeBase.h" // 後で敵の情報を取得する際に使用

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
        // ブラックボードを初期化し、ビヘイビアツリーを開始します
        BlackboardComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
        BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);
    }
}