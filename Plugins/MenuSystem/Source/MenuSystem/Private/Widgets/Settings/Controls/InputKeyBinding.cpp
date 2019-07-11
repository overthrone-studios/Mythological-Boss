// Copyright Ali El Saleh 2019

#include "InputKeyBinding.h"
#include "WidgetTree.h"
#include "InputKeySelector.h"

void UInputKeyBinding::Init()
{
	Super::Init();

	DefaultPrimaryInput = CurrentPrimaryInput;
	DefaultGamepadInput = CurrentGamepadInput;

	PrimaryKeySelector = Cast<UInputKeySelector>(WidgetTree->FindWidget(FName("PrimaryKeySelector")));
	GamepadKeySelector = Cast<UInputKeySelector>(WidgetTree->FindWidget(FName("GamepadKeySelector")));
}

void UInputKeyBinding::Reset()
{
	CurrentPrimaryInput = DefaultPrimaryInput;
	CurrentGamepadInput = DefaultGamepadInput;

	SetDefaultInput(PrimaryKeySelector, GamepadKeySelector);
}

bool UInputKeyBinding::IsDefault()
{
	return PrimaryKeySelector->SelectedKey == DefaultPrimaryInput && GamepadKeySelector->SelectedKey == DefaultGamepadInput;
}

void UInputKeyBinding::SetDefaultInput(class UInputKeySelector* Primary, class UInputKeySelector* Gamepad)
{
	DefaultPrimaryInput = CurrentPrimaryInput;
	DefaultGamepadInput = CurrentGamepadInput;

	Primary->SetSelectedKey(DefaultPrimaryInput);
	Gamepad->SetSelectedKey(DefaultGamepadInput);
}
