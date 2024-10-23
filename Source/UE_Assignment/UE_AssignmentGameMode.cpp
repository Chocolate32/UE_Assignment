// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_AssignmentGameMode.h"
#include "UE_AssignmentCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE_AssignmentGameMode::AUE_AssignmentGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
