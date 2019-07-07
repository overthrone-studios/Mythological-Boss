// Copyright Ali El Saleh 2019

#include "OptionsMenu.h"
#include "MenuHUD.h"
#include "TimerManager.h"
#include "ButtonBase.h"
#include "VideoMenu.h"
#include "AudioMenu.h"
#include "ControlsMenu.h"
#include "MainMenu.h"

void UOptionsMenu::Init()
{
	Super::Init();

	InitializeButtons();
}

void UOptionsMenu::Forward(const EButtonType Menu)
{
	MenuHUD->HideMenu(StaticClass());

	Super::Forward(Menu);
}

void UOptionsMenu::GoForward()
{
	switch (MenuSelected)
	{
	case BTN_VIDEO:
		MenuHUD->ShowMenu(UVideoMenu::StaticClass());
		break;

	case BTN_AUDIO:
		MenuHUD->ShowMenu(UAudioMenu::StaticClass());
		break;

	case BTN_CONTROLS:
		MenuHUD->ShowMenu(UControlsMenu::StaticClass());
		break;

	case BTN_BACK:
		MenuHUD->ShowMenu(UMainMenu::StaticClass());
		break;

	default:
		break;
	}

	Super::GoForward();
}

void UOptionsMenu::Back()
{
	MenuHUD->HideMenu(StaticClass());

	Super::Back();
}

void UOptionsMenu::GoBack()
{
	MenuHUD->ShowMenu(UMainMenu::StaticClass());

	Super::GoBack();
}

