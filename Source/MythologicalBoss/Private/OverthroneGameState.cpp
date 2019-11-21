// Copyright Overthrone Studios 2019

#include "OverthroneGameState.h"

#include "OverthroneFunctionLibrary.h"

#include "Boss/Mordath.h"

#include "Kismet/GameplayStatics.h"
#include "PlatformFilemanager.h"
#include "FileHelper.h"
#include "Log.h"

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
	OnGamePaused.Broadcast(IsGamePaused());
}

void AOverthroneGameState::UnPauseGame() const
{
	UOverthroneFunctionLibrary::UnPauseGame(this, PlayerController);
	OnGamePaused.Broadcast(IsGamePaused());
}

void AOverthroneGameState::StartRecordingPlayerActions()
{
	bCanRecordPlayerData = true;
}

void AOverthroneGameState::StopRecordingPlayerActions()
{
	bCanRecordPlayerData = false;
}

void AOverthroneGameState::UpdatePlayerActionCount(const FString& ActionName, const int32 Count)
{
	if (ActionName == "Light Attack")
		PlayerData.LightAttacks = Count;
	else if (ActionName == "Heavy Attack")
		PlayerData.HeavyAttacks = Count;
	else if (ActionName == "Charge Attack")
		PlayerData.ChargeAttacks = Count;
	else if (ActionName == "Dash Attack")
		PlayerData.DashAttacks = Count;
	else if (ActionName == "Dash")
		PlayerData.Dashes = Count;
	else if (ActionName == "Block")
		PlayerData.Blocks = Count;
	else if (ActionName == "Parry")
		PlayerData.Parries = Count;
}

void AOverthroneGameState::UpdatePlayerActionCount(const EAttackType_Player AttackType, const int32 Count)
{
	switch (AttackType)
	{
		case ATP_Light:
			PlayerData.LightAttacks = Count;
		break;

		case ATP_Heavy:
			PlayerData.HeavyAttacks = Count;
		break;
		
		case ATP_Charge:
			PlayerData.ChargeAttacks = Count;
		break;
		
		case ATP_Dash:
			PlayerData.DashAttacks = Count;
		break;
		
		default:
		break;
	}
}

void AOverthroneGameState::SavePlayerActions() const
{
	const FString SaveDirectory = FString(FPaths::ProjectDir());
	const FString FileName = FString("PlayerActions.sav");

	// Get absolute file path
	const FString AbsoluteFilePath = SaveDirectory + FileName;

	FString TextToSave = "Light Attack: " + FString::FromInt(PlayerData.LightAttacks);
	TextToSave.Append(FString("\r\nHeavy Attack: ") + FString::FromInt(PlayerData.HeavyAttacks));
	TextToSave.Append(FString("\r\nCharge Attack: ") + FString::FromInt(PlayerData.ChargeAttacks));
	TextToSave.Append(FString("\r\nDash Attack: ") + FString::FromInt(PlayerData.DashAttacks));
	TextToSave.Append(FString("\r\nDashes: ") + FString::FromInt(PlayerData.Dashes));
	TextToSave.Append(FString("\r\nBlocks: ") + FString::FromInt(PlayerData.Blocks));
	TextToSave.Append(FString("\r\nParries: ") + FString::FromInt(PlayerData.Parries));

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		if (FFileHelper::SaveStringToFile(TextToSave, *AbsoluteFilePath))
		{
			//ULog::Success("File saved! " + AbsoluteFilePath, true);
		}
	}
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
