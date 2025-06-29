// Fill out your copyright notice in the Description page of Project Settings.


#include "TextContainer.h"
#include "Misc/Paths.h"

// Sets default values
ATextContainer::ATextContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATextContainer::BeginPlay()
{
	Super::BeginPlay();
    LoadCSVData();
}

// Called every frame
void ATextContainer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATextContainer::LoadCSVData()
{

}

FUIStruct* ATextContainer::OutputText(FName name)
{
    // データテーブル取得
    FUIStruct* aRecord = aDataTable->FindRow<FUIStruct>(name, "");
    return aRecord;
}