// Copyright Overthrone Studios 2019

#include "Public/OverthroneHUD.h"
#include "Public/OverthroneFunctionLibrary.h"
#include "Public/OverthroneGameInstance.h"
#include "Widgets/HUD/MasterHUD.h"
#include "Widget.h"
#include "WidgetTree.h"
#include "UserWidget.h"
#include "ConstructorHelpers.h"
#include "Log.h"

AOverthroneHUD::AOverthroneHUD()
{
	static ConstructorHelpers::FClassFinder<UWidget> MasterHUDWidget(TEXT("WidgetBlueprint'/Game/UI/PlayerHUD/UI_MasterPlayerHUD.UI_MasterPlayerHUD_C'"));
	static ConstructorHelpers::FClassFinder<UWidget> IntroCutsceneWidget(TEXT("WidgetBlueprint'/Game/UI/Cutscenes/UI_IntroCutscene.UI_IntroCutscene_C'"));

	if (MasterHUDWidget.Succeeded())
		HUDWidgetClass = MasterHUDWidget.Class;

	if (IntroCutsceneWidget.Succeeded())
		IntroCutsceneWidgetClass = IntroCutsceneWidget.Class;
}

void AOverthroneHUD::Init()
{
	GameInstance = UOverthroneFunctionLibrary::GetGameInstance(this);
	
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
	IntroCutsceneWidget = CreateWidget<UUserWidget>(GetWorld(), IntroCutsceneWidgetClass, FName("Intro Cutscene Widget"));
}

void AOverthroneHUD::AddWidgetsToScreen()
{
	MasterHUD->AddToViewport();
	MasterHUD->SetVisibility(ESlateVisibility::Visible);

	IntroCutsceneWidget->AddToViewport();
	IntroCutsceneWidget->SetVisibility(ESlateVisibility::Visible);
}

void AOverthroneHUD::InitWidgets()
{
	MasterHUD->Init();

#if !UE_BUILD_SHIPPING
	MasterHUD->SwitchToHUDIndex(0);
#else
	IntroCutsceneWidget->SetVisibility(ESlateVisibility::Visible);

	MasterHUD->SwitchToHUDIndex(2);

	GameInstance->PauseGame();
#endif
}
