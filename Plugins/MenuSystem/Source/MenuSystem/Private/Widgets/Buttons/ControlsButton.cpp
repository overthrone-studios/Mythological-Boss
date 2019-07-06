// Copyright Ali El Saleh 2019

#include "ControlsButton.h"
#include "ControlsMenu.h"
#include "MenuHUD.h"
#include "Log.h"

void UControlsButton::Init()
{
	Super::Init();

	Menu = Cast<UControlsMenu>(MenuHUD->GetMenu(CONTROLS_MENU));
	ControlsMenu = Cast<UControlsMenu>(Menu);
}

void UControlsButton::OnButtonReleased()
{
	Super::OnButtonReleased();

	// Error check
	if (IsMenuNull())
		return;

	switch (ButtonType)
	{
	case BTN_RESET_INPUT:
		ControlsMenu->ResetKeyBindings();
		break;

	case BTN_BACK:
		Menu->Back();
		break;

	default:
		ULog::LogDebugMessage(WARNING, FString(GetName() + " | Button not implemented!"), true);
		break;
	}
}

