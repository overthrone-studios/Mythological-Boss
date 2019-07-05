// Copyright Ali El Saleh 2019

#include "MenuHUD.h"
#include "ConstructorHelpers.h"
#include "Widget.h"
#include "UserWidget.h"
#include "MainMenu.h"
#include "MasterMenu.h"

AMenuHUD::AMenuHUD()
{
	static ConstructorHelpers::FClassFinder<UWidget> MasterMenuUI(TEXT("WidgetBlueprint'/Game/UI/UI_MasterMenu.UI_MasterMenu_C'"));

	if (MasterMenuUI.Succeeded())
		WidgetClass = MasterMenuUI.Class;
}

UUserWidget* AMenuHUD::GetMenu(const int32 Index)
{
	//return MasterMenu->GetMenus()[Index];
	return nullptr;
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

void AMenuHUD::ShowMenu(const int32 Index)
{
	//if (Index < MasterMenu->GetMenus().Num())
	//{
	//	MasterMenu->GetMenu(Index)->FadeIn();
	//	MasterMenu->SwitchToMenuIndex(Index);
	//}
	//else
	//	UDebug::LogDebugMessage(ERROR, FString("Index out of range"), true);
}

void AMenuHUD::HideMenu(const int32 Index)
{
	//if (Index < MasterMenu->GetMenus().Num())
	//	MasterMenu->GetMenu(Index)->FadeOut();
	//else
	//	ULogStatics::LogDebugMessage(ERROR, FString("Index out of range"), true);
}

void AMenuHUD::SlideMainMenu()
{
	//Cast<UMainMenu>(MasterMenu->GetMenu(MAIN_MENU))->SlideOut();
}

void AMenuHUD::BeginPlay()
{
	//CreateWidgets();
	//
	//// Error check for newley created widgets
	//if (AllWidgetsValid())
	//{
	//	AddWidgetsToScreen();
	//	InitializeWidgets();
	//}
	//else
	//	LogWidgetFailures();
}

void AMenuHUD::CreateWidgets()
{
	MasterMenu = CreateWidget<UMasterMenu>(GetWorld(), WidgetClass, FName("MasterMenuUI"));
}

void AMenuHUD::InitializeWidgets()
{
	//MasterMenu->Init();
}

void AMenuHUD::AddWidgetsToScreen()
{
	MasterMenu->AddToViewport();
	MasterMenu->SetVisibility(ESlateVisibility::Visible);
}

void AMenuHUD::LogWidgetFailures()
{
	//if (!MasterMenu)
	//	ULogStatics::LogDebugMessage(ERROR, FString("Failed to create master menu widget"), true);
}

bool AMenuHUD::AllWidgetsValid()
{
	return MasterMenu != nullptr;
}

