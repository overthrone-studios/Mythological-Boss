// Copyright Ali El Saleh 2019

#include "OptionsMenu.h"
#include "MenuHUD.h"
#include "VideoMenu.h"
#include "MainMenu.h"


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

