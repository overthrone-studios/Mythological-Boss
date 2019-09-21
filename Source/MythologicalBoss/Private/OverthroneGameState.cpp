// Copyright Overthrone Studios 2019

#include "OverthroneGameState.h"

#include "OverthroneFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

void AOverthroneGameState::EnterBossStage(const EBossStage InBossStage)
{
	switch (InBossStage)
	{
	case Stage_1:
		break;

	case Stage_2:
		break;

	case Stage_3:
		break;
	}
}

void AOverthroneGameState::RestartGame()
{
	UOverthroneFunctionLibrary::UnPauseGame(PlayerController);

	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, *LevelName);

	PlayerData.ResetData();
	BossData.ResetData();
}

bool AOverthroneGameState::IsGamePaused()
{
	return UGameplayStatics::IsGamePaused(this);
}

void AOverthroneGameState::PauseGame()
{
	UOverthroneFunctionLibrary::PauseGame(PlayerController);
}

void AOverthroneGameState::UnPauseGame()
{
	UOverthroneFunctionLibrary::UnPauseGame(PlayerController);
}

void AOverthroneGameState::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
}
