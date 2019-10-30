// Copyright Overthrone Studios 2019

#include "TutorialGameMode.h"
#include "OverthroneHUD.h"
#include "TutorialHUD.h"
#include "GameFramework/Character.h"
#include "UserWidget.h"
#include "ConstructorHelpers.h"
#include "Log.h"
#include "Kismet/GameplayStatics.h"
#include "OverthroneGameInstance.h"
#include "TimerManager.h"

ATutorialGameMode::ATutorialGameMode()
{
	static ConstructorHelpers::FClassFinder<ACharacter> BP_YlvaTutorial(TEXT("Blueprint'/Game/Characters/Ylva/BP_YlvaTutorial.BP_YlvaTutorial_C'"));
	static ConstructorHelpers::FClassFinder<UWidget> TutorialHUDWidget(TEXT("WidgetBlueprint'/Game/UI/Tutorial/UI_TutorialHUD.UI_TutorialHUD_C'"));

	if (BP_YlvaTutorial.Succeeded())
		DefaultPawnClass = BP_YlvaTutorial.Class;

	if (TutorialHUDWidget.Succeeded())
		TutorialHUDClass = TutorialHUDWidget.Class;

	HUDClass = AOverthroneHUD::StaticClass();
}

void ATutorialGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitHUD();

	// Initialize our game instance
	const auto GameInstance = Cast<UOverthroneGameInstance>(UGameplayStatics::GetGameInstance(this));

	if (GameInstance)
		GameInstance->InitInstance();
	else
		ULog::ObjectValidity(GameInstance, true);

	GetWorldTimerManager().SetTimer(TutorialStartTimer, this, &ATutorialGameMode::StartTutorial, 2.0f);
}

void ATutorialGameMode::StartTutorial()
{
	TutorialHUD->ProgressTutorial();
	TutorialHUD->FadeIn();

	bTutorialStarted = true;
}

void ATutorialGameMode::InitHUD()
{
	const auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// Initialize the main HUD
	const auto OverthroneHUD = Cast<AOverthroneHUD>(PlayerController->GetHUD());

	if (OverthroneHUD)
		OverthroneHUD->Init();
	else
		ULog::ObjectValidity(OverthroneHUD, true);

	TutorialHUD = CreateWidget<UTutorialHUD>(GetWorld(), TutorialHUDClass, FName("Tutorial HUD"));
	TutorialHUD->AddToViewport();
	TutorialHUD->Init();
}
