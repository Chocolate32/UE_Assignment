// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpSpawner.h"
#include "PickUpActor.h"

// Sets default values
APickUpSpawner::APickUpSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

//// Called when the game starts or when spawned
//void APickUpSpawner::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

// Called every frame
void APickUpSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (canSpawn) {
		if (timeTracked >= SpawnDuration) {

			UWorld* const world = GetWorld();
			if (world != nullptr) {

				FVector SpawnLocation = SpawnPoints[FMath::Rand() % SpawnPoints.Num()] + GetActorLocation();

				world->SpawnActor<APickUpActor>(PickUpClass, SpawnLocation, FRotator::ZeroRotator)->SetSpawnerActor(this);
				
				canSpawn = false;
			}

			timeTracked -= SpawnDuration;
		}
		timeTracked += DeltaTime;
	}
}

void APickUpSpawner::AllowSpawn() {

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "It's about to be destroyed");

	canSpawn = true;
}

void APickUpSpawner::EndLifeCycle() {
	Destroy();
}