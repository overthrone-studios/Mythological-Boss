// Copyright Overthrone Studios 2019

#include "Public/BossBattleGameMode.h"
#include "Public/OverthroneHUD.h"
#include "Player/Ylva.h"

ABossBattleGameMode::ABossBattleGameMode()
{
	// Set default pawn class to our blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Player/ThirdPersonCharacter"));

	//if (PlayerPawnBPClass.Class)
	//{
		DefaultPawnClass = AYlva::StaticClass();
	//}

	// Set default HUD class to our PlayerHUD
	HUDClass = AOverthroneHUD::StaticClass();
}
