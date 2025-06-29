// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TextContainer.generated.h"

USTRUCT(BlueprintType)
struct FUIStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI",DisplayName = "ID")
	int id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", DisplayName = "STRING")
	FText ofletter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", DisplayName = "SPEED")
	float outputRate;
};


UCLASS()
class ANTIGRAVITY_API ATextContainer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATextContainer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:

	UFUNCTION(BlueprintCallable, Category = "CSVReader")
	void LoadCSVData();

	FUIStruct* OutputText(FName name);

private:
	// データテーブル
	UDataTable* aDataTable;
};
