// Copyright Ali El Saleh 2019

#include "MainMenu.h"
#include "MenuHUD.h"
#include "ButtonBase.h"
#include "WidgetTree.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "WidgetAnimation.h"
#include "TimerManager.h"
#include "NewGameMenu.h"
#include "OptionsMenu.h"
#include "Log.h"
#include "LevelSelectMenu.h"

void UMainMenu::Init()
{
	Super::Init();

	InitializeButtons();

	WidgetTree->FindWidget("NewGame")->SetKeyboardFocus();
}

void UMainMenu::SlideOut()
{
	if (!Slide)
	{
		ULog::DebugMessage(WARNING, FString("Slide animation has not been assigned"), true);
		return;
	}

	PlayAnimation(Slide);

	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UMainMenu::Remove, 1.0f, false, Slide->GetEndTime());
}

void UMainMenu::Forward(const EButtonType Button)
{
	MenuHUD->HideMenu(StaticClass());

	Super::Forward(Button);
}

void UMainMenu::GoForward()
{
	switch (MenuSelected)
	{
	case BTN_NEW_GAME:
		MenuHUD->EnableGameInputMode();
		UGameplayStatics::OpenLevel(GetWorld(), MapToOpen);
		break;

	case BTN_LVL_SELECT:
		MenuHUD->ShowMenu(ULevelSelectMenu::StaticClass());
		break;

	case BTN_FEATS:
		//MenuHUD->ShowMenu(UFeatsMenu::StaticClass());
		break;

	case BTN_OPTIONS:
		MenuHUD->ShowMenu(UOptionsMenu::StaticClass());
		break;

	case BTN_EXIT:
		UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit);
		break;

	default:
		MenuHUD->ShowMenu(StaticClass());
		break;
	}

	Super::GoForward();
}

void UMainMenu::Remove()
{
	MenuHUD->HideHUD();
}