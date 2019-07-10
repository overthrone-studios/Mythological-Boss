// Copyright Ali El Saleh 2019

#include "ControlsMenu.h"
#include "MenuHUD.h"
#include "WidgetTree.h"
#include "OutputDeviceNull.h"
#include "OptionsMenu.h"

void UControlsMenu::Init()
{
	Super::Init();

	ResetWarningBox = Cast<UUserWidget>(WidgetTree->FindWidget(FName("ResetWarningBox")));

	InitializeButtons();

	InitializeSettings();

	HideWidgets();
}

void UControlsMenu::InitializeButtons()
{
	Super::InitializeButtons();

	// Initialise the buttons in the reset warning box
	const auto YesButton = Cast<UButtonBase>(ResetWarningBox->WidgetTree->FindWidget(FName("Yes")));
	const auto NoButton = Cast<UButtonBase>(ResetWarningBox->WidgetTree->FindWidget(FName("No")));
	
	YesButton->Init();
	NoButton->Init();
}

void UControlsMenu::Back()
{
	MenuHUD->HideMenu(StaticClass());

	Super::Back();
}

void UControlsMenu::GoBack()
{
	MenuHUD->ShowMenu(UOptionsMenu::StaticClass());

	Super::GoBack();
}

void UControlsMenu::ShowResetWarning()
{
	ResetWarningBox->SetVisibility(ESlateVisibility::Visible);
}

void UControlsMenu::HideResetWarning()
{
	ResetWarningBox->SetVisibility(ESlateVisibility::Hidden);
}

void UControlsMenu::HideWidgets()
{
	ResetWarningBox->SetVisibility(ESlateVisibility::Hidden);
}

void UControlsMenu::ResetKeyBindings()
{
	for (auto Slot : ControlsSlots)
	{
		// Call function in BP to reset key mapping to default
		FOutputDeviceNull OutputDevice;
		Slot->CallFunctionByNameWithArguments(TEXT("ResetKeyMappings"), OutputDevice, nullptr, true);
	}
}
