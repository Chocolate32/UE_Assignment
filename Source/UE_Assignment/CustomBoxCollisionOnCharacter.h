// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "CustomBoxCollisionOnCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterEnterCollision, class AUE_AssignmentCharacter*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterLeavesCollision, class AUE_AssignmentCharacter*, Player);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE_ASSIGNMENT_API UCustomBoxCollisionOnCharacter : public UBoxComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Interaction)
	FOnCharacterEnterCollision OnCharacterCollided;
	
	UPROPERTY(BlueprintAssignable, Category = Interaction)
	FOnCharacterLeavesCollision OnCharacterLeftCollision;

	//UCustomBoxCollisionOnCharacter();


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
