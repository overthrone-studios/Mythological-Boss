// Copyright Overthrone Studios 2019

#include "Managers/OverthroneTutorialManager.h"
#include "UserWidget.h"
#include "TutorialHUD.h"
#include "Log.h"
#include "Kismet/GameplayStatics.h"

AOverthroneTutorialManager::AOverthroneTutorialManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AOverthroneTutorialManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	TutorialHUD = CreateWidget<UTutorialHUD>(PlayerController, TutorialHUDClass, "Tutorial HUD Widget");
	if (!TutorialHUD)
		ULog::Error("Tutorial HUD is null", true);
}

void AOverthroneTutorialManager::BeginTutorial()
{
	TutorialHUD->AddToViewport();
	TutorialHUD->SetVisibility(ESlateVisibility::Visible);
	PlayerController->SetInputMode(FInputModeUIOnly());

	TutorialHUD->ProgressTutorial();
	TutorialHUD->FadeIn();
}

void AOverthroneTutorialManager::EndTutorial()
{
	PlayerController->SetInputMode(FInputModeGameOnly());

	TutorialHUD->EndTutorial();

	Destroy();
}

void AOverthroneTutorialManager::NextTutorial()
{
	PlayerController->SetInputMode(FInputModeUIOnly());

	TutorialHUD->ProgressTutorial();
}
