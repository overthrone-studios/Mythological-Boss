// Copyright Overthrone Studios 2019

#include "OverthroneGameState.h"

#include "OverthroneFunctionLibrary.h"

#include "Boss/Mordath.h"

#include "Kismet/GameplayStatics.h"

void AOverthroneGameState::EnterBossStage(const EBossStage_Mordath InBossStage) const
{
	switch (InBossStage)
	{
	case Stage_1:
		Mordath->EnterStage(Stage_1);
	break;

	case Stage_2:
		Mordath->EnterStage(Stage_2);
	break;

	case Stage_3:
		Mordath->EnterStage(Stage_3);
	break;

	default:
		Mordath->EnterStage(Stage_1);
	break;
	}
}

void AOverthroneGameState::LockBoss() const
{
	Mordath->ToggleLockSelf();
}

void AOverthroneGameState::KillMordath() const
{
	Mordath->Die();
}

void AOverthroneGameState::SpawnMordathGhost() const
{
	Mordath->SpawnGhost();
}

void AOverthroneGameState::RestartGame()
{
	UOverthroneFunctionLibrary::UnPauseGame(this, PlayerController);

	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, *LevelName);

	PlayerData.ResetData();
	BossData.ResetData();
}

bool AOverthroneGameState::IsGamePaused() const
{
	return UGameplayStatics::IsGamePaused(this);
}

void AOverthroneGameState::PauseGame() const
{
	UOverthroneFunctionLibrary::PauseGame(this, PlayerController);
}

void AOverthroneGameState::UnPauseGame() const
{
	UOverthroneFunctionLibrary::UnPauseGame(this, PlayerController);
}

bool AOverthroneGameState::IsBossTeleporting() const
{
	return Mordath->IsTeleporting();
}

void AOverthroneGameState::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
}
