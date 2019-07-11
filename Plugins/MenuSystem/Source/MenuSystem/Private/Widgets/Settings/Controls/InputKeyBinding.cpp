// Copyright Ali El Saleh 2019

#include "InputKeyBinding.h"
#include "WidgetTree.h"
#include "InputKeySelector.h"
#include "ControlsMenu.h"

void UInputKeyBinding::Init()
{
	Super::Init();

	DefaultPrimaryInput = CurrentPrimaryInput;
	DefaultGamepadInput = CurrentGamepadInput;

	PrimaryKeySelector = Cast<UInputKeySelector>(WidgetTree->FindWidget(FName("PrimaryKeySelector")));
	GamepadKeySelector = Cast<UInputKeySelector>(WidgetTree->FindWidget(FName("GamepadKeySelector")));

	ControlsMenu = Cast<UControlsMenu>(Menu);
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

void UInputKeyBinding::UpdatePrimaryInput(const FInputChord& NewInput)
{
	ControlsMenu->UpdateInputMapping(this, CurrentPrimaryInput, NewInput);

	CurrentPrimaryInput = NewInput;
}

void UInputKeyBinding::UpdateGamepadInput(const FInputChord& NewInput)
{
	ControlsMenu->UpdateInputMapping(this, CurrentGamepadInput, NewInput);

	CurrentGamepadInput = NewInput;
}
