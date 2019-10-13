// Copyright Overthrone Studios 2019

#include "OverthroneGameState.h"

#include "OverthroneFunctionLibrary.h"

#include "Boss/Mordath.h"

#include "Kismet/GameplayStatics.h"

void AOverthroneGameState::EnterBossStage(const EBossStage_Mordath InBossStage)
{
	switch (InBossStage)
	{
	case Stage_1:
		Boss->EnterStage(Stage_1);
	break;

	case Stage_2:
		Boss->EnterStage(Stage_2);
	break;

	case Stage_3:
		Boss->EnterStage(Stage_3);
	break;
	}
}

void AOverthroneGameState::LockBoss()
{
	Boss->ToggleLockSelf();
}

void AOverthroneGameState::RestartGame()
{
	UOverthroneFunctionLibrary::UnPauseGame(this, PlayerController);

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
	UOverthroneFunctionLibrary::PauseGame(this, PlayerController);
}

void AOverthroneGameState::UnPauseGame()
{
	UOverthroneFunctionLibrary::UnPauseGame(this, PlayerController);
}

void AOverthroneGameState::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
}
