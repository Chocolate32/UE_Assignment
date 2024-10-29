// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpSpawner.generated.h"

UCLASS()
class UE_ASSIGNMENT_API APickUpSpawner : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APickUpActor> PickUpClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Config, meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	TArray<FVector> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
	float SpawnDuration{ 10.f };

	float timeTracked{ 0.f };

	bool canSpawn{ true };


public:	
	void AllowSpawn();
	// Sets default values for this actor's properties
	APickUpSpawner();

//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void EndLifeCycle();

};
