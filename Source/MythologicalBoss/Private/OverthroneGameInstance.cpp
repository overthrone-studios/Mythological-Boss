// Copyright Overthrone Studios 2019

#include "OverthroneGameInstance.h"

#include "LockOn.h"

#include "Misc/FeatData.h"

#include "Kismet/GameplayStatics.h"

#include "Log.h"
#include "CoreDelegates.h"
static bool bFirstLaunch = true;

UOverthroneGameInstance::UOverthroneGameInstance()
{
}

void UOverthroneGameInstance::Init()
{
	FCoreDelegates::OnControllerConnectionChange.AddUFunction(this, "OnControllerConnectionChanged");
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

void UOverthroneGameInstance::OnFeatAchieved()
{
	AchievedFeat->bIsComplete = true;

	ULog::Success(AchievedFeat->Title.ToString() + " feat has been completed!", true);
}

void UOverthroneGameInstance::OnControllerConnectionChanged(bool bIsConnected, int32 UserID, int32 UserIndex)
{
	OnGamepadConnectionUpdated.Broadcast(bIsConnected);
}
