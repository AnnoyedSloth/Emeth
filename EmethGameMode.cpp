// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "EmethGameMode.h"
#include "EmethCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine.h"

AEmethGameMode::AEmethGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Player/MyPlayer_Blueprint"));
	if (PlayerPawnClassFinder.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	}
}
