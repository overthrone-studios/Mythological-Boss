// Copyright Ali El Saleh 2019

#include "OptionsMenu.h"
#include "MenuHUD.h"
#include "TimerManager.h"
#include "ButtonBase.h"
#include "WidgetTree.h"

void UOptionsMenu::Init()
{
	Super::Init();

	InitializeButtons();
}

void UOptionsMenu::Forward(const EButtonType Menu)
{
	MenuHUD->HideMenu(OPTIONS_MENU);

	Super::Forward(Menu);
}

void UOptionsMenu::GoForward()
{
	switch (MenuSelected)
	{
	case BTN_VIDEO:
		MenuHUD->ShowMenu(VIDEO_MENU);
		break;

	case BTN_AUDIO:
		MenuHUD->ShowMenu(AUDIO_MENU);
		break;

	case BTN_CONTROLS:
		MenuHUD->ShowMenu(CONTROLS_MENU);
		break;

	case BTN_BACK:
		MenuHUD->ShowMenu(MAIN_MENU);
		break;

	default:
		break;
	}

	Super::GoForward();
}

void UOptionsMenu::Back()
{
	MenuHUD->HideMenu(OPTIONS_MENU);

	Super::Back();
}

void UOptionsMenu::GoBack()
{
	MenuHUD->ShowMenu(MAIN_MENU);

	Super::GoBack();
}

