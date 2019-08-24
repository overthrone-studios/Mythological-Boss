// Copyright Overthrone Studios 2019

#include "LevelSelectMenu.h"
#include "MenuHUD.h"
#include "MainMenu.h"

void ULevelSelectMenu::Forward(const EButtonType Menu)
{
	MenuHUD->HideMenu(StaticClass());

	Super::Forward(Menu);
}

void ULevelSelectMenu::GoForward()
{
	switch (MenuSelected)
	{
	case BTN_BACK:
		MenuHUD->ShowMenu(UMainMenu::StaticClass());
		break;

	default:
		break;
	}

	Super::GoForward();
}

void ULevelSelectMenu::Back()
{
	MenuHUD->HideMenu(StaticClass());

	Super::Back();
}

void ULevelSelectMenu::GoBack()
{
	MenuHUD->ShowMenu(UMainMenu::StaticClass());

	Super::GoBack();
}
