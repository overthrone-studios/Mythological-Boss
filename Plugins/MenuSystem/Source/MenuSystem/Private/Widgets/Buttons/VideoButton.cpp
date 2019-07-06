// Copyright Ali El Saleh 2019

#include "VideoButton.h"
#include "MenuHUD.h"
#include "VideoMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Log.h"

void UVideoButton::Init()
{
	Super::Init();

	Menu = Cast<UVideoMenu>(MenuHUD->GetMenu(VIDEO_MENU));
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

	case BTN_BACK:
		Menu->Back();
		break;

	default:
		ULog::LogDebugMessage(WARNING, FString(GetName() + " | Button not implemented!"), true);
		break;
	}
}


