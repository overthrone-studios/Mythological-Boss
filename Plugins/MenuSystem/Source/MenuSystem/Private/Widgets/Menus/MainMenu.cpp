// Copyright Ali El Saleh 2019

#include "MainMenu.h"
#include "MenuHUD.h"
#include "WidgetTree.h"
#include "WidgetAnimation.h"
#include "TimerManager.h"
#include "NewGameMenu.h"
#include "Log.h"

void UMainMenu::Init()
{
	Super::Init();

	WidgetTree->FindWidget("NewGame")->SetKeyboardFocus();
}

void UMainMenu::SlideOut()
{
	if (!Slide)
	{
		ULog::DebugMessage(WARNING, FString("Slide animation has not been assigned"), true);
		return;
	}

	PlayAnimation(Slide);

	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UMainMenu::Remove, 1.0f, false, Slide->GetEndTime());
}

void UMainMenu::Remove()
{
	MenuHUD->HideHUD();
}