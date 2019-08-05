// Copyright Ali El Saleh 2019

#include "VideoButton.h"
#include "MenuHUD.h"
#include "VideoMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Log.h"

void UVideoButton::Init()
{
	Super::Init();

	Menu = Cast<UVideoMenu>(MenuHUD->GetMenu(UVideoMenu::StaticClass()));
	VideoMenu = Cast<UVideoMenu>(Menu);
}

void UVideoButton::OnButtonReleased()
{
	Super::OnButtonReleased();

	// Error check
	if (IsMenuNull())
		return;

	switch (ButtonType)
	{
	case BTN_APPLY:
		Menu->Apply();
		break;

	case BTN_RESET:
		if (!Menu->AreAllSettingsDefault())
			VideoMenu->ShowResetWarning();
		break;

	case BTN_RESET_YES:
		Menu->Reset();
		VideoMenu->HideResetWarning();
		break;

	case BTN_RESET_NO:
		VideoMenu->HideResetWarning();
		break;

	case BTN_DISCARD:
		VideoMenu->DiscardChanges();
		break;

	case BTN_BACK:
		VideoMenu->Back();
		break;

	default:
		ULog::DebugMessage(WARNING, FString(GetName() + " | Button not implemented!"), true);
		break;
	}
}


