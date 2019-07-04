// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MythologicalBossGameMode.h"
#include "MythologicalBossCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMythologicalBossGameMode::AMythologicalBossGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
