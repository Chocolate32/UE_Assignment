// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpActor.generated.h"

class UTP_PickUpComponent;
class UBillboardComponent;

UCLASS()
class UE_ASSIGNMENT_API APickUpActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
	UTP_PickUpComponent* PickUpCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* Billboard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config, meta = (AllowPrivateAccess = "true"))
	class APickUpSpawner* SpawnerActor{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	USoundBase* SpawnSound;

	float angle{ 0.f };
public:	
	// Sets default values for this actor's properties
	APickUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Config)
	float radius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
	float FloatingStrengh{ 0.2f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Config)
	float FloatingSpeed{ 3.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	UTexture2D *PickUpImage;

	void SetSpawnerActor(class APickUpSpawner*);
};
