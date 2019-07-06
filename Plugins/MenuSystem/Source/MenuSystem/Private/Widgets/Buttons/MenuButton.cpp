// Copyright Ali El Saleh 2019

#include "MenuButton.h"
#include "MainMenu.h"
#include "MenuHUD.h"

void UMenuButton::Init()
{
	Super::Init();

	Menu = Cast<UMainMenu>(MenuHUD->GetMenu(MAIN_MENU));
	MainMenu = Cast<UMainMenu>(Menu);
}

void UMenuButton::OnButtonReleased()
{
	Super::OnButtonReleased();

	// Error check
	if (IsMenuNull())
		return;

	Menu->Forward(ButtonType);
}

