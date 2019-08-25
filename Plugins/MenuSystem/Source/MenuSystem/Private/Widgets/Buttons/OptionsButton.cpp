// Copyright Ali El Saleh 2019

#include "OptionsButton.h"
#include "OptionsMenu.h"
#include "MenuHUD.h"

void UOptionsButton::Init()
{
	Super::Init();

	Menu = Cast<UOptionsMenu>(MenuHUD->GetMenu(UOptionsMenu::StaticClass()));
	OptionsMenu = Cast<UOptionsMenu>(Menu);
}
