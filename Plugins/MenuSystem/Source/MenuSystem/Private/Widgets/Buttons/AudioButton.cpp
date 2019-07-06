// Copyright Ali El Saleh 2019


#include "AudioButton.h"
#include "MenuHUD.h"
#include "AudioMenu.h"
#include "Log.h"

void UAudioButton::Init()
{
	Super::Init();

	Menu = Cast<UAudioMenu>(MenuHUD->GetMenu(AUDIO_MENU));
	AudioMenu = Cast<UAudioMenu>(Menu);
}

void UAudioButton::OnButtonReleased()
{
	Super::OnButtonReleased();

	// Error check
	if (IsMenuNull())
		return;

	switch (ButtonType)
	{
	case BTN_BACK:
		Menu->Back();
		break;

	default:
		ULog::LogDebugMessage(WARNING, FString(GetName() + " | Button not implemented!"), true);
		break;
	}
}