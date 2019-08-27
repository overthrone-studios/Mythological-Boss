// Copyright Overthrone Studios 2019

#include "OverthroneGameInstance.h"
#include "UserWidget.h"
#include "LockOn.h"
#include "Misc/FeatData.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "Log.h"

UOverthroneGameInstance::UOverthroneGameInstance()
{
	static ConstructorHelpers::FClassFinder<UWidget> PauseMenuWidget(TEXT("WidgetBlueprint'/Game/UI/Menus/UI_PauseMenu.UI_PauseMenu_C'"));

	if (PauseMenuWidget.Succeeded())
		PauseMenuWidgetClass = PauseMenuWidget.Class;

	bFirstLaunch = true;
}

void UOverthroneGameInstance::RestartGame()
{
	UnPauseGame();

	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, *LevelName);
}

bool UOverthroneGameInstance::IsGamePaused()
{
	return UGameplayStatics::IsGamePaused(this);
}

void UOverthroneGameInstance::PauseGame()
{
	PauseMenu->SetVisibility(ESlateVisibility::Visible);
	PauseMenu->SetKeyboardFocus();

	SetInputModeUI();

	UGameplayStatics::SetGamePaused(this, true);
}

void UOverthroneGameInstance::UnPauseGame()
{
	PauseMenu->SetVisibility(ESlateVisibility::Hidden);

	SetInputModeGame();

	UGameplayStatics::SetGamePaused(this, false);
}

void UOverthroneGameInstance::SetInputModeUI() const
{
	const FInputModeUIOnly InputModeUIOnly;
	PlayerController->SetInputMode(InputModeUIOnly);
	PlayerController->bShowMouseCursor = true;
}

void UOverthroneGameInstance::SetInputModeGame() const
{
	const FInputModeGameOnly InputModeGameOnly;
	PlayerController->SetInputMode(InputModeGameOnly);
	PlayerController->bShowMouseCursor = false;
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
	for (auto Feat : Feats)
	{
		Feat->CurrentCount = 0;
		Feat->bIsComplete = false;
	}
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

	PauseMenu = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetClass, FName("Pause Menu"));
	PauseMenu->AddToViewport();
	PauseMenu->SetVisibility(ESlateVisibility::Hidden);

	if (!bFirstLaunch)
		return;

	InitFeats();

	bFirstLaunch = false;
}

void UOverthroneGameInstance::SetLockOnLocation(const FVector& LockOnLocation) const
{
	if (LockOn)
		LockOn->SetActorLocation(LockOnLocation);
}

void UOverthroneGameInstance::SetLockOnRotation(const FRotator& LockOnRotation) const
{
	if (LockOn)
		LockOn->SetActorRotation(LockOnRotation);
}

void UOverthroneGameInstance::ToggleLockOnVisibility(const bool bIsVisible) const
{
	if (LockOn)
		LockOn->SetActorHiddenInGame(!bIsVisible);
}

void UOverthroneGameInstance::OnFeatAchieved()
{
	AchievedFeat->bIsComplete = true;

	ULog::Success(AchievedFeat->Title.ToString() + " feat has been completed!", true);
}
