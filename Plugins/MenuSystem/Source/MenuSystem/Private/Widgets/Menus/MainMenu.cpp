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
		ULog::LogDebugMessage(WARNING, FString("Slide animation has not been assigned"), true);
		return;
	}

	PlayAnimation(Slide);

	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UMainMenu::Remove, 1.0f, false, Slide->GetEndTime());
}

void UMainMenu::Forward(const EButtonType Button)
{
	if (Button == BTN_EXIT)
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit);
		return;
	}

	if (Button == BTN_CONTINUE)
	{
		MenuHUD->SlideMainMenu();
		return;
	}

	MenuHUD->HideMenu(StaticClass());

	Super::Forward(Button);
}

void UMainMenu::GoForward()
{
	switch (MenuSelected)
	{
	case BTN_NEW_GAME:
		MenuHUD->EnableGameInputMode();
		if (!MapToOpen.ToString().IsEmpty())
			UGameplayStatics::OpenLevel(GetWorld(), MapToOpen);
		//MenuHUD->ShowMenu(UNewGameMenu::StaticClass());
		break;

	case BTN_CONTINUE:
		MenuHUD->SlideMainMenu();
		break;

	case BTN_OPTIONS:
		MenuHUD->ShowMenu(UOptionsMenu::StaticClass());
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