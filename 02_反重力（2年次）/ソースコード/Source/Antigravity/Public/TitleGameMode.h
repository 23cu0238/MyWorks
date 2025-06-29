// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "TitleGameMode.generated.h"


/**
 * 
 */

class UInputComponent;

UCLASS()
class ANTIGRAVITY_API ATitleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATitleGameMode();

private:
	// 入力コンポーネントを設定する
	void SetupInputComponent(class UInputComponent* PlayerInputComponent);

	void SelectButton();

	void PressButton();

	/*UInputMappingContext* mTitleMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* mButtonNavigation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* mConfirmAction;*/
};
