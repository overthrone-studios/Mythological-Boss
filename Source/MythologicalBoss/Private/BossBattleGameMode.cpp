// Copyright Overthrone Studios 2019

#include "Public/BossBattleGameMode.h"
#include "ConstructorHelpers.h"

ABossBattleGameMode::ABossBattleGameMode()
{
	// Set default pawn class to our blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Player/ThirdPersonCharacter"));

	if (PlayerPawnBPClass.Class)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
