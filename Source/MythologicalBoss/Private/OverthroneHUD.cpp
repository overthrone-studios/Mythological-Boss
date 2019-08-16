// Copyright Overthrone Studios 2019

#include "Public/OverthroneHUD.h"
#include "Widgets/HUD/MasterHUD.h"
#include "Widget.h"
#include "WidgetTree.h"
#include "UserWidget.h"
#include "ConstructorHelpers.h"
#include "Log.h"

AOverthroneHUD::AOverthroneHUD()
{
	static ConstructorHelpers::FClassFinder<UWidget> MasterHUDWidget(TEXT("WidgetBlueprint'/Game/UI/PlayerHUD/UI_MasterPlayerHUD.UI_MasterPlayerHUD_C'"));

	if (MasterHUDWidget.Succeeded())
		HUDWidgetClass = MasterHUDWidget.Class;
}

void AOverthroneHUD::Init()
{
	CreateWidgets();

	if (MasterHUD)
	{
		AddWidgetsToScreen();
		InitWidgets();
	}
	else
	{
		ULog::DebugMessage(ERROR, "Failed to create the MasterHUD widget!", true);
	}
}

void AOverthroneHUD::CreateWidgets()
{
	MasterHUD = CreateWidget<UMasterHUD>(GetWorld(), HUDWidgetClass, FName("Master Player HUD"));
}

void AOverthroneHUD::AddWidgetsToScreen()
{
	MasterHUD->AddToViewport();
	MasterHUD->SetVisibility(ESlateVisibility::Visible);
}

void AOverthroneHUD::InitWidgets()
{
	MasterHUD->Init();

#if !UE_BUILD_SHIPPING
	MasterHUD->SwitchToHUDIndex(0);
#else
	MasterHUD->SwitchToHUDIndex(2);
	MasterHUD->HideTitle();
	MasterHUD->HideBoxes();
#endif
}
