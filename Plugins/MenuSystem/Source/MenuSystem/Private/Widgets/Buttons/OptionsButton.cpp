// Copyright Ali El Saleh 2019

#include "OptionsButton.h"
#include "OptionsMenu.h"
#include "MenuHUD.h"

void UOptionsButton::Init()
{
	Super::Init();

	Menu = Cast<UOptionsMenu>(MenuHUD->GetMenu(OPTIONS_MENU));
	OptionsMenu = Cast<UOptionsMenu>(Menu);
}

void UOptionsButton::OnButtonReleased()
{
	Super::OnButtonReleased(); 

	// Error check
	if (IsMenuNull())
		return;

	OptionsMenu->Forward(ButtonType);
}
