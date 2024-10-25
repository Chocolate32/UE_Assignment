// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairParent.generated.h"

/**
 * 
 */
UCLASS()
class UE_ASSIGNMENT_API UCrosshairParent : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CrosshairUpdate(const int &val);
};
