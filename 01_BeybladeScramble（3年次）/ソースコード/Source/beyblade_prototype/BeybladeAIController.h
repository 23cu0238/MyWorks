// BeybladeAIController.h

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BeybladeAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class BEYBLADE_PROTOTYPE_API ABeybladeAIController : public AAIController
{
    GENERATED_BODY()

public:
    ABeybladeAIController();

protected:
    // ���̃R���g���[���[���|�[�����|�[�b�V�����������ɌĂ΂�܂�
    virtual void OnPossess(APawn* InPawn) override;

public:
    // AI�̎v�l���W�b�N���`�����r�w�C�r�A�c���[
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBehaviorTreeComponent* BehaviorTreeComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBlackboardComponent* BlackboardComponent;
};