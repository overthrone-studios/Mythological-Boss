// Copyright Ali El Saleh 2019

#include "AudioMenu.h"
#include "MenuHUD.h"

void UAudioMenu::Init()
{
	Super::Init();

	InitializeButtons();
}

void UAudioMenu::Back()
{
	MenuHUD->HideMenu(AUDIO_MENU);

	Super::Back();
}

void UAudioMenu::GoBack()
{
	MenuHUD->ShowMenu(OPTIONS_MENU);

	Super::GoBack();
}
