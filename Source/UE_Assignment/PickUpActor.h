// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpActor.generated.h"

class UTP_PickUpComponent;
class UStaticMeshComponent;

UCLASS()
class UE_ASSIGNMENT_API APickUpActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
	UTP_PickUpComponent* PickUpCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PickUpMesh;
public:	
	// Sets default values for this actor's properties
	APickUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Config)
	float radius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
	float meshRotationSpeed;
};
