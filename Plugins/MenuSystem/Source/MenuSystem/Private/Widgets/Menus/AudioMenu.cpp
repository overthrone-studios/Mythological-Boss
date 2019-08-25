// Copyright Ali El Saleh 2019

#include "AudioMenu.h"
#include "MenuHUD.h"
#include "OptionsMenu.h"

void UAudioMenu::Init()
{
	Super::Init();

	InitializeSettings();
}

void UAudioMenu::Back()
{
	MenuHUD->HideMenu(StaticClass());

	Super::Back();
}

void UAudioMenu::GoBack()
{
	MenuHUD->ShowMenu(UOptionsMenu::StaticClass());

	Super::GoBack();
}
