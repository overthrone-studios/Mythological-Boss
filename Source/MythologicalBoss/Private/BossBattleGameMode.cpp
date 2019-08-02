// Copyright Overthrone Studios 2019

#include "Public/BossBattleGameMode.h"
#include "Public/OverthroneHUD.h"
#include "Player/Ylva.h"
#include "Kismet/GameplayStatics.h"

ABossBattleGameMode::ABossBattleGameMode()
{
	DefaultPawnClass = AYlva::StaticClass();

	// Set default HUD class to our HUD
	HUDClass = AOverthroneHUD::StaticClass();
}

void ABossBattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	const auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	const auto OverthroneHUD = Cast<AOverthroneHUD>(PlayerController->GetHUD());

	if (OverthroneHUD)
		OverthroneHUD->Init();
}
