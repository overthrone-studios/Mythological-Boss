// Copyright Ali El Saleh 2019

#include "ControlsButton.h"
#include "ControlsMenu.h"
#include "MenuHUD.h"
#include "Log.h"

void UControlsButton::Init()
{
	Super::Init();

	Menu = Cast<UControlsMenu>(MenuHUD->GetMenu(UControlsMenu::StaticClass()));
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
	case BTN_RESET:
		if (!Menu->AreAllSettingsDefault())
			ControlsMenu->ShowResetWarning();
		break;

	case BTN_RESET_YES:
		ControlsMenu->Reset();
		ControlsMenu->HideResetWarning();
		break;

	case BTN_RESET_NO:
		ControlsMenu->HideResetWarning();
		break;

	case BTN_BACK:
		Menu->Back();
		break;

	default:
		ULog::DebugMessage(WARNING, FString(GetName() + " | Button not implemented!"), true);
		break;
	}
}

