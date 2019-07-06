// Copyright Ali El Saleh 2019

#include "VideoMenu.h"
#include "GameFramework/GameUserSettings.h"
#include "MenuHUD.h"
#include "TimerManager.h"
#include "WidgetTree.h"

void UVideoMenu::Init()
{
	Super::Init();

	InitializeSettings();

	InitializeButtons();

	Apply();
}

void UVideoMenu::Back()
{
	MenuHUD->HideMenu(VIDEO_MENU);

	Super::Back();
}

void UVideoMenu::GoBack()
{
	MenuHUD->ShowMenu(OPTIONS_MENU);

	Super::GoBack();
}