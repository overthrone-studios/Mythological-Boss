// Copyright Ali El Saleh 2019

#include "MenuHUD.h"
#include "ConstructorHelpers.h"
#include "Widget.h"
#include "UserWidget.h"
#include "MainMenu.h"
#include "MasterMenu.h"
#include "Log.h"

AMenuHUD::AMenuHUD()
{
	static ConstructorHelpers::FClassFinder<UWidget> MasterMenuUI(TEXT("WidgetBlueprint'/Game/UI/Menus/UI_MasterMenu.UI_MasterMenu_C'"));

	if (MasterMenuUI.Succeeded())
		WidgetClass = MasterMenuUI.Class;
}

UUserWidget* AMenuHUD::GetMenu(const TSubclassOf<UMenuBase> MenuClass)
{
	return MasterMenu->GetMenu(MenuClass);
}

void AMenuHUD::ShowHUD()
{
	Super::ShowHUD();

	MasterMenu->AddToViewport();
}

void AMenuHUD::HideHUD()
{
	MasterMenu->RemoveFromViewport();
}

void AMenuHUD::ShowMenu(const TSubclassOf<class UMenuBase> MenuClass)
{
	const auto Menu = MasterMenu->GetMenu(MenuClass);

	if (Menu)
	{
		MasterMenu->SwitchToMenu(Menu);
		Menu->FadeIn();
	}
	else
	{
		ULog::LogDebugMessage(ERROR, FString("Cannot show menu because it does not exist within the master menu"), true);
	}
}

void AMenuHUD::HideMenu(const TSubclassOf<class UMenuBase> MenuClass)
{
	MasterMenu->GetMenu(MenuClass)->FadeOut();
}

void AMenuHUD::SlideMainMenu()
{
	const auto Menu = MasterMenu->GetMenu(UMainMenu::StaticClass());

	if (Menu)
		Cast<UMainMenu>(Menu)->SlideOut();
	else
		ULog::LogDebugMessage(ERROR, FString("Cannot slide menu because it does not exist within the master menu"), true);
}

void AMenuHUD::BeginPlay()
{
	CreateWidgets();
	
	// Error check for newly created widgets
	if (AllWidgetsValid())
	{
		AddWidgetsToScreen();
		InitializeWidgets();
	}
	else
		LogWidgetFailures();
}

void AMenuHUD::CreateWidgets()
{
	MasterMenu = CreateWidget<UMasterMenu>(GetWorld(), WidgetClass, FName("MasterMenuUI"));
}

void AMenuHUD::InitializeWidgets()
{
	MasterMenu->Init();
}

void AMenuHUD::AddWidgetsToScreen()
{
	MasterMenu->AddToViewport();
	MasterMenu->SetVisibility(ESlateVisibility::Visible);
}

void AMenuHUD::LogWidgetFailures()
{
	if (!MasterMenu)
		ULog::LogDebugMessage(ERROR, FString("Failed to create master menu widget"), true);
}

bool AMenuHUD::AllWidgetsValid()
{
	return MasterMenu != nullptr;
}

