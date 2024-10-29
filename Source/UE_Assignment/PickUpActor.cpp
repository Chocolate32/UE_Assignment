// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"
#include "TP_PickUpComponent.h"
#include "PickUpSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BillboardComponent.h"

// Sets default values
APickUpActor::APickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickUpCollision = CreateDefaultSubobject < UTP_PickUpComponent> (TEXT("SphereCollision"));
	PickUpCollision->SetupAttachment(GetRootComponent());
	PickUpCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Image"));
	Billboard->SetupAttachment(PickUpCollision);
	Billboard->SetHiddenInGame(false);
	if (PickUpImage != nullptr)	Billboard->SetSprite(PickUpImage);


}

// Called when the game starts or when spawned
void APickUpActor::BeginPlay()
{
	Super::BeginPlay();

	//PickUpCollision->AddLocalOffset(FVector(0.f, 0.f, radius));

	PickUpCollision->UpdateRadius(radius);

	if (PickUpImage != nullptr)		Billboard->SetSprite(PickUpImage);

	if (SpawnSound != nullptr)		UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation());
}

// Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	angle += DeltaTime * FloatingSpeed;
	

	Billboard->AddRelativeLocation(FVector(0.f, 0.f, FMath::Cos(angle) * FloatingStrengh));

	if (angle > 360.f)	angle -= 360.f;
}

void APickUpActor::SetSpawnerActor(class APickUpSpawner* actor) {
	SpawnerActor = actor;
}

void APickUpActor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	if (SpawnerActor != nullptr)
		SpawnerActor->AllowSpawn();
}