// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityObject.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
AGravityObject::AGravityObject(const FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->RemoveFromRoot();
	GetCapsuleComponent()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	//GetCapsuleComponent()->DestroyComponent();


	/*USkeletalMeshComponent* mesh = GetMesh();
	mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	RootComponent = mesh;*/

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;

	//Super::GetArrowComponent()->DestroyComponent();
	GetMesh()->DestroyComponent();



}

// Called when the game starts or when spawned
void AGravityObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGravityObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGravityObject::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

