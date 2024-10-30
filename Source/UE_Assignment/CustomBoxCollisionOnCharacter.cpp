// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomBoxCollisionOnCharacter.h"
#include "UE_AssignmentCharacter.h"

void UCustomBoxCollisionOnCharacter::BeginPlay() {
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UCustomBoxCollisionOnCharacter::OnBoxBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UCustomBoxCollisionOnCharacter::OnBoxEndOverlap);
}

void UCustomBoxCollisionOnCharacter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AUE_AssignmentCharacter* Character = Cast<AUE_AssignmentCharacter>(OtherActor);

	if (Character != nullptr) {
		OnCharacterCollided.Broadcast(Character);
	}
}

void UCustomBoxCollisionOnCharacter::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	AUE_AssignmentCharacter* Character = Cast<AUE_AssignmentCharacter>(OtherActor);

	if (Character != nullptr) {
		OnCharacterLeftCollision.Broadcast(Character);
	}
}