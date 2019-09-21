// Copyright Overthrone Studios 2019

#include "Public/BossBattleGameMode.h"
#include "Public/OverthroneHUD.h"
#include "Public/OverthroneGameInstance.h"
#include "Player/Ylva.h"
#include "Kismet/GameplayStatics.h"
#include "Log.h"
#include "OverthroneGameState.h"

ABossBattleGameMode::ABossBattleGameMode()
{
	DefaultPawnClass = AYlva::StaticClass();

	// Set default HUD class to our HUD
	HUDClass = AOverthroneHUD::StaticClass();
	GameStateClass = AOverthroneGameState::StaticClass();
}

void ABossBattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitHUD();
	InitGameInstance();
}

void ABossBattleGameMode::InitHUD()
{
	const auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// Initialize the main HUD
	const auto OverthroneHUD = Cast<AOverthroneHUD>(PlayerController->GetHUD());

	if (OverthroneHUD)
		OverthroneHUD->Init();
	else
		ULog::ObjectValidity(OverthroneHUD, true);
}

void ABossBattleGameMode::InitGameInstance()
{
	// Initialize our game instance
	const auto GameInstance = Cast<UOverthroneGameInstance>(UGameplayStatics::GetGameInstance(this));

	if (GameInstance)
		GameInstance->InitInstance();
	else
		ULog::ObjectValidity(GameInstance, true);
}
