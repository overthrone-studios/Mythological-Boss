// Copyright Overthrone Studios 2019

#include "OverthroneGameInstance.h"
#include "Kismet/GameplayStatics.h"

UOverthroneGameInstance::UOverthroneGameInstance()
{
}

bool UOverthroneGameInstance::IsGamePaused()
{
	return UGameplayStatics::IsGamePaused(this);
}

void UOverthroneGameInstance::PauseGame()
{
	UGameplayStatics::SetGamePaused(this, true);
}

void UOverthroneGameInstance::UnPauseGame()
{
	UGameplayStatics::SetGamePaused(this, false);
}
