// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameMode.h"
#include "GameFramework/GameUserSettings.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

ATitleGameMode::ATitleGameMode()
{
	
}

void ATitleGameMode::SetupInputComponent(class UInputComponent* PlayerInputComponent)
{
	//if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	//{
	//	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	//	{
	//		Subsystem->AddMappingContext(mTitleMappingContext, 0);
	//	}

	//	
	//}

	//if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

	//	// 
	//	EnhancedInputComponent->BindAction(mConfirmAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	//	
	//	// 
	//	EnhancedInputComponent->BindAction(mButtonNavigation, ETriggerEvent::Triggered, this, &AAntigravityCharacter::Move);
	//}
	//else
	//{
	//	UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	//}
}

void ATitleGameMode::SelectButton()
{
}

void ATitleGameMode::PressButton()
{
}

