// Copyright Ali El Saleh 2019

#include "LevelSelectMenu.h"
#include "MenuHUD.h"
#include "MainMenu.h"

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
