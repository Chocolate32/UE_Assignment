// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"
#include "TP_PickUpComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APickUpActor::APickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickUpCollision = CreateDefaultSubobject < UTP_PickUpComponent> (TEXT("SphereCollision"));
	PickUpCollision->SetupAttachment(GetRootComponent());

	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	PickUpMesh->SetupAttachment(PickUpCollision);

}

// Called when the game starts or when spawned
void APickUpActor::BeginPlay()
{
	Super::BeginPlay();

	//PickUpCollision->AddLocalOffset(FVector(0.f, 0.f, radius));

	PickUpCollision->UpdateRadius(radius);
}

// Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PickUpMesh->AddWorldRotation(FRotator(0.f, meshRotationSpeed/100.f, 0.f));
}

