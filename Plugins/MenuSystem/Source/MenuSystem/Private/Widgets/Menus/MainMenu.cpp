// Copyright Ali El Saleh 2019

#include "MainMenu.h"
#include "MenuHUD.h"
#include "ButtonBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "WidgetAnimation.h"
#include "TimerManager.h"

void UMainMenu::Init()
{
	Super::Init();

	InitializeButtons();
}

void UMainMenu::SlideOut()
{
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

	MenuHUD->HideMenu(MAIN_MENU);

	Super::Forward(Button);
}

void UMainMenu::GoForward()
{
	switch (MenuSelected)
	{
	case BTN_NEW_GAME:
		MenuHUD->ShowMenu(NEW_GAME_MENU);
		break;

	case BTN_CONTINUE:
		MenuHUD->SlideMainMenu();
		break;

	case BTN_OPTIONS:
		MenuHUD->ShowMenu(OPTIONS_MENU);
		break;

	default:
		MenuHUD->ShowMenu(MAIN_MENU);
		break;
	}

	Super::GoForward();
}

void UMainMenu::Remove()
{
	MenuHUD->HideHUD();
}