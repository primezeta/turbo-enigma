// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "FirstPerson.h"
#include "FirstPersonGameMode.h"
#include "FirstPersonHUD.h"
#include "FirstPersonCharacter.h"

AFirstPersonGameMode::AFirstPersonGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFirstPersonHUD::StaticClass();
}
