// Copyright Overthrone Studios 2019

#include "OverthroneGameInstance.h"
#include "UserWidget.h"
#include "LockOn.h"
#include "Misc/FeatData.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "Log.h"
#include "OverthroneFunctionLibrary.h"

UOverthroneGameInstance::UOverthroneGameInstance()
{
	bFirstLaunch = true;
}

void UOverthroneGameInstance::RestartGame()
{
	UOverthroneFunctionLibrary::UnPauseGame(PlayerController);

	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, *LevelName);

	PlayerData.bIsDead = false;
	BossData.bIsDead = false;
}

bool UOverthroneGameInstance::IsGamePaused()
{
	return UGameplayStatics::IsGamePaused(this);
}

void UOverthroneGameInstance::PauseGame()
{
	UOverthroneFunctionLibrary::PauseGame(PlayerController);
}

void UOverthroneGameInstance::UnPauseGame()
{
	UOverthroneFunctionLibrary::UnPauseGame(PlayerController);
}

UFeatData* UOverthroneGameInstance::GetFeat(const FString& FeatName)
{
	for (auto Feat : Feats)
	{
		if (Feat->Title.ToString() == FeatName)
			return Feat;
	}

	ULog::Warning("Could not find " + FeatName + ". You may have misspelled it. Make sure the " + FeatName + " Feat is added to the array in " + GetName(), true);
	return nullptr;
}

void UOverthroneGameInstance::ResetFeats()
{
	// Check if we have any completed feats
	int32 CompletedFeats = 0;
	for (auto Feat : Feats)
	{
		if (Feat->bIsComplete)
			CompletedFeats++;
	}

	if (CompletedFeats == 0)
		return;

	// Reset all feats
	for (auto Feat : Feats)
	{
		Feat->CurrentCount = 0;
		Feat->bIsComplete = false;
	}

	ULog::Success("All feats reset!", true);
}

void UOverthroneGameInstance::InitFeats()
{
	for (auto Feat : Feats)
	{
		if (!Feat->OnFeatAchieved.IsBound())
			Feat->OnFeatAchieved.AddDynamic(this, &UOverthroneGameInstance::OnFeatAchieved);
	}
}

void UOverthroneGameInstance::InitInstance()
{
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (!bFirstLaunch)
		return;

	InitFeats();

	bFirstLaunch = false;
}

void UOverthroneGameInstance::ToggleLockOnVisibility(const bool bIsVisible) const
{
	if (LockOn)
		LockOn->ToggleVisibility(!bIsVisible);
}

void UOverthroneGameInstance::OnFeatAchieved()
{
	AchievedFeat->bIsComplete = true;

	ULog::Success(AchievedFeat->Title.ToString() + " feat has been completed!", true);
}
