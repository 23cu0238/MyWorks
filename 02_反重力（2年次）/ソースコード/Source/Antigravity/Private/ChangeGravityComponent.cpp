// ChangeGravityComponentの実装
// 詳しくはヘッダーファイルを参照

#include "ChangeGravityComponent.h"

#include "GameFramework/Character.h"
#include "PlayerCharacter.h"
#include "GravityController.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include"PlayerAttack.h"
#include"PlayerAttackRange.h"
// Sets default values for this component's properties
UChangeGravityComponent::UChangeGravityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UChangeGravityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



//重力を変える対象を設定します
void UChangeGravityComponent::SetTarget(ACharacter* target)
{
	mTarget = target;
}
//重力を変更する実行関数
void UChangeGravityComponent::ChangeGravity(const FHitResult& gravityPoint)
{
	if (mTarget == nullptr)
	{
		return;
	}
	mTarget->GetCharacterMovement()->SetGravityDirection(UKismetMathLibrary::GetDirectionUnitVector(gravityPoint.ImpactNormal * 100.0f + gravityPoint.ImpactPoint, gravityPoint.ImpactPoint));
}
//変更し終わったら呼ぶかも
void UChangeGravityComponent::OnExit()
{

}