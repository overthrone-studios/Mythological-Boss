// Copyright Ali El Saleh 2019

#include "VideoMenu.h"
#include "GameFramework/GameUserSettings.h"
#include "MenuHUD.h"
#include "OptionsMenu.h"

void UVideoMenu::Init()
{
	Super::Init();

	InitializeSettings();

	InitializeButtons();

	Apply();
}

void UVideoMenu::Back()
{
	MenuHUD->HideMenu(StaticClass());

	Super::Back();
}

void UVideoMenu::GoBack()
{
	MenuHUD->ShowMenu(UOptionsMenu::StaticClass());

	Super::GoBack();
}