// Copyright Ali El Saleh 2019

#include "InputKeyBinding.h"
#include "WidgetTree.h"
#include "InputKeySelector.h"
#include "ControlsMenu.h"
#include "Log.h"

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

void UInputKeyBinding::SetSelectedPrimaryInput(UInputKeySelector* Primary)
{
	if (!Primary)
		return;

	Primary->SetSelectedKey(CurrentPrimaryInput);
}

void UInputKeyBinding::SetSelectedGamepadInput(UInputKeySelector* Gamepad)
{
	if (!Gamepad)
		return;

	Gamepad->SetSelectedKey(CurrentGamepadInput);
}

void UInputKeyBinding::UpdatePrimaryInput(const FInputChord& NewInput)
{
	if (IsInputAGamepadKey(NewInput))
	{
		SetSelectedPrimaryInput(PrimaryKeySelector);
		return;
	}

	ControlsMenu->UpdateInputMapping(this, CurrentPrimaryInput, NewInput);

	CurrentPrimaryInput = NewInput;
}

void UInputKeyBinding::UpdateGamepadInput(const FInputChord& NewInput)
{
	if (!IsInputAGamepadKey(NewInput))
	{
		SetSelectedGamepadInput(GamepadKeySelector);
		return;
	}

	ControlsMenu->UpdateInputMapping(this, CurrentGamepadInput, NewInput);

	CurrentGamepadInput = NewInput;
}

bool UInputKeyBinding::IsInputAGamepadKey(const FInputChord& NewInput)
{
	TArray<FKey> AllKeys;
	TArray<FKey> GamepadKeys;

	// Get all gamepad keys
	EKeys::GetAllKeys(AllKeys);
	for (const auto& Key : AllKeys)
	{
		const FName KeyName = Key.GetFName();

		if (KeyName.ToString().Contains("Gamepad"))
			GamepadKeys.Add(Key);
	}

	// Check if the new input is in fact a gamepad key
	for (const auto& GamepadKey : GamepadKeys)
	{
		if (NewInput.Key == GamepadKey) // If true, NewInput is not a gamepad key!
		{
			return true;
		}
	}

	return false;
}
