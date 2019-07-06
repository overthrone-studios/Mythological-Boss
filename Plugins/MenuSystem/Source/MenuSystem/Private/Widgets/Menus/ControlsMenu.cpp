// Copyright Ali El Saleh 2019

#include "ControlsMenu.h"
#include "MenuHUD.h"
#include "WidgetTree.h"
#include "OutputDeviceNull.h"

void UControlsMenu::Init()
{
	Super::Init();

	InitializeButtons();
}

void UControlsMenu::Back()
{
	MenuHUD->HideMenu(CONTROLS_MENU);

	Super::Back();
}

void UControlsMenu::GoBack()
{
	MenuHUD->ShowMenu(OPTIONS_MENU);

	Super::GoBack();
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
