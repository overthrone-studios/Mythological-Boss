// Copyright Ali El Saleh 2019

#include "ControlsMenu.h"
#include "MenuHUD.h"
#include "WidgetTree.h"
#include "MenuSetting.h"
#include "OptionsMenu.h"
#include "InputKeyBinding.h"
#include "GameFramework/InputSettings.h"
#include "ConfigCacheIni.h"
#include "InvertSetting.h"
#include "Button.h"
#include "CanvasPanel.h"
#include "Log.h"

void UControlsMenu::Init()
{
	Super::Init();

	ResetWarningBox = Cast<UCanvasPanel>(WidgetTree->FindWidget(FName("ResetWarningBox")));
	DuplicateWarningBox = Cast<UUserWidget>(WidgetTree->FindWidget(FName("DuplicateWarningBox")));
	BackButton = Cast<UButton>(WidgetTree->FindWidget(FName("Back")));
	MouseInvertXSetting = Cast<UInvertSetting>(WidgetTree->FindWidget(FName("MouseInvertX")));
	MouseInvertYSetting = Cast<UInvertSetting>(WidgetTree->FindWidget(FName("MouseInvertY")));
	GamepadInvertXSetting = Cast<UInvertSetting>(WidgetTree->FindWidget(FName("GamepadInvertX")));
	GamepadInvertYSetting = Cast<UInvertSetting>(WidgetTree->FindWidget(FName("GamepadInvertY")));

	InitializeSettings();

	InitializeControls();

	HideWidgets();
}

void UControlsMenu::Back()
{
	ApplyInvertSettings();

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

void UControlsMenu::RemoveAllKeyBindings()
{
	// Get access to the input settings
	const auto Input = const_cast<UInputSettings*>(GetDefault<UInputSettings>());

	// Get the current array of axis mappings from input settings
	Input->AxisMappings.Empty();

	// Get the current array of action mappings from input settings
	Input->ActionMappings.Empty();
}

void UControlsMenu::RebindInputMapping(const class UInputKeyBinding* InputKeyBinding, const FInputChord& OldInput, const FInputChord& NewInput)
{
	// Get access to the input settings
	const auto Input = const_cast<UInputSettings*>(GetDefault<UInputSettings>());

	const FName InputName = InputKeyBinding->GetInputName();

	if (InputKeyBinding->IsAxis())
	{
		// Remove the old input
		FInputAxisKeyMapping OldAxisMapping;
		OldAxisMapping.AxisName = InputName;
		OldAxisMapping.Key = OldInput.Key;
		OldAxisMapping.Scale = InputKeyBinding->GetScale();
		Input->RemoveAxisMapping(OldAxisMapping);

		// Add the new input
		FInputAxisKeyMapping NewAxisMapping;
		NewAxisMapping.AxisName = InputName;

		if (NewInput.Key == EKeys::Gamepad_RightStick_Up || NewInput.Key == EKeys::Gamepad_RightStick_Down)
			NewAxisMapping.Key = EKeys::Gamepad_LeftY;
		else if (NewInput.Key == EKeys::Gamepad_RightStick_Left || NewInput.Key == EKeys::Gamepad_RightStick_Right)
			NewAxisMapping.Key = EKeys::Gamepad_LeftX;
		else
			NewAxisMapping.Key = NewInput.Key;

		NewAxisMapping.Scale = InputKeyBinding->GetScale();
		Input->AddAxisMapping(NewAxisMapping);
	}
	else
	{
		// Remove the old input
		FInputActionKeyMapping OldActionMapping;
		OldActionMapping.ActionName = InputName;
		OldActionMapping.Key = OldInput.Key;
		Input->RemoveActionMapping(OldActionMapping);

		// Add the new input
		FInputActionKeyMapping NewActionMapping;
		NewActionMapping.ActionName = InputName;
		NewActionMapping.Key = NewInput.Key;
		Input->AddActionMapping(NewActionMapping);
	}

	Input->SaveKeyMappings();
	Input->ForceRebuildKeymaps();
}

TArray<UInputKeyBinding*> UControlsMenu::GetAllControls()
{
	TArray<UInputKeyBinding*> FoundControls;

	for (auto Setting : MenuSettings)
	{
		if (Setting->IsA(UInputKeyBinding::StaticClass()))
			FoundControls.Add(Cast<UInputKeyBinding>(Setting));
	}

	return FoundControls;
}

void UControlsMenu::InitializeControls()
{
	// Get access to the input settings
	const auto Input = const_cast<UInputSettings*>(GetDefault<UInputSettings>());

	// Get the current array of axis mappings from input settings
	const auto AxisMappings = Input->AxisMappings;

	// Get the current array of action mappings from input settings
	const auto ActionMappings = Input->ActionMappings;

	// If number of mappings is greater than the default engine input mappings, then load our inputs
	if (AxisMappings.Num() > 6 || ActionMappings.Num() > 10)
		return;

	// Lambda function to add an axis mapping to input settings
	const auto AddAxisMapping = [&](const FName& AxisName, const FKey& Key, const float Scale)
	{
		FInputAxisKeyMapping AxisMapping;
		AxisMapping.AxisName = AxisName;
		AxisMapping.Key = Key;
		AxisMapping.Scale = Scale;
		Input->AddAxisMapping(AxisMapping);
	};

	// Lambda function to add an action mapping to input settings
	const auto AddActionMapping = [&](const FName& ActionName, const FKey& Key)
	{
		FInputActionKeyMapping ActionMapping;
		ActionMapping.ActionName = ActionName;
		ActionMapping.Key = Key;
		Input->AddActionMapping(ActionMapping);
	};

	// Get all the input key binding settings in this menu and assign to input settings
	const auto Controls = GetAllControls();
	for (auto Control : Controls)
	{
		const FName InputName = Control->GetInputName();
		const FInputChord DefaultPrimaryInput = Control->GetCurrentPrimaryInput();
		const FInputChord DefaultGamepadInput = Control->GetCurrentGamepadInput();
		const float AxisScale = Control->GetScale();
	
		// Add this control to input settings
		if (Control->IsAxis())
		{
			AddAxisMapping(InputName, DefaultPrimaryInput.Key, AxisScale);

			if (DefaultGamepadInput.Key.IsFloatAxis() && AxisScale > 0.0f)
				AddAxisMapping(InputName, DefaultGamepadInput.Key, AxisScale);
		}
		else
		{
			AddActionMapping(InputName, DefaultPrimaryInput.Key);
			AddActionMapping(InputName, DefaultGamepadInput.Key);
		}
	}

	// Add the standard default controls for mouse and gamepad
	AddAxisMapping(FName("Turn"), EKeys::MouseX, MouseInvertXSetting->IsInvertEnabled() ? -1.0f : 1.0f);
	AddAxisMapping(FName("LookUp"), EKeys::MouseY, MouseInvertYSetting->IsInvertEnabled() ? 1.0f : -1.0f);
	AddAxisMapping(FName("TurnRate"), EKeys::Gamepad_RightX, GamepadInvertXSetting->IsInvertEnabled() ? -1.0f : 1.0f);
	AddAxisMapping(FName("LookUpRate"), EKeys::Gamepad_RightY, GamepadInvertYSetting->IsInvertEnabled() ? -1.0f : 1.0f);
	AddActionMapping(FName("Pause"), EKeys::Escape);
	AddActionMapping(FName("Pause"), EKeys::Gamepad_Special_Right);

	// Save to input config file (DefaultInput.ini)
	Input->SaveKeyMappings();

	// Update in Project Settings -> Engine -> Input
	Input->ForceRebuildKeymaps();
}

void UControlsMenu::Reset()
{
	ResetKeyBindings();
	ResetKeyBindings();
}

void UControlsMenu::HideWidgets()
{
	ResetWarningBox->SetVisibility(ESlateVisibility::Hidden);
	DuplicateWarningBox->SetVisibility(ESlateVisibility::Hidden);
}

void UControlsMenu::ResetKeyBindings()
{
	RemoveAllKeyBindings();

	Super::Reset();

	InitializeControls();
}

void UControlsMenu::ApplyInvertSettings()
{
	// Get access to the input settings
	const auto Input = const_cast<UInputSettings*>(GetDefault<UInputSettings>());

	// Lambda function to add an axis mapping to input settings
	const auto UpdateAxisMapping = [&](const FName& AxisName, const FKey& Key, const float Scale)
	{
		FInputAxisKeyMapping AxisMapping;
		AxisMapping.AxisName = AxisName;
		AxisMapping.Key = Key;
		AxisMapping.Scale = Scale;
		Input->RemoveAxisMapping(AxisMapping);
		Input->AddAxisMapping(AxisMapping);
	};

	// Add the standard default controls for mouse and gamepad
	UpdateAxisMapping(FName("Turn"), EKeys::MouseX, MouseInvertXSetting->IsInvertEnabled() ? -1.0f : 1.0f);
	UpdateAxisMapping(FName("LookUp"), EKeys::MouseY, MouseInvertYSetting->IsInvertEnabled() ? 1.0f : -1.0f);
	UpdateAxisMapping(FName("TurnRate"), EKeys::Gamepad_RightX, GamepadInvertXSetting->IsInvertEnabled() ? -1.0f : 1.0f);
	UpdateAxisMapping(FName("LookUpRate"), EKeys::Gamepad_RightY, GamepadInvertYSetting->IsInvertEnabled() ? -1.0f : 1.0f);

	// Save to config file
	Input->SaveKeyMappings();

	// Update in Project Settings -> Engine -> Input
	Input->ForceRebuildKeymaps();
}

bool UControlsMenu::IsPrimaryInputKeyDuplicate(UInputKeyBinding* ControlToCheck, const FInputChord& InputToCheck)
{
	const auto Controls = GetAllControls();
	for (auto Control : Controls)
	{
		// If this control setting is not the same as ControlToCheck AND If we have a duplicate key
		if (Control != ControlToCheck && Control->GetSelectedPrimaryKey() == InputToCheck)
		{
			if (Control->GetInputName() == ControlToCheck->GetInputName())
			{
				const auto ConflictedInput = Control->GetCurrentPrimaryInput();
				const auto OldInput = ControlToCheck->GetCurrentPrimaryInput();

				RebindInputMapping(Control, Control->GetCurrentPrimaryInput(), OldInput);
				RebindInputMapping(ControlToCheck, ControlToCheck->GetCurrentPrimaryInput(), ConflictedInput);
				
				ControlToCheck->SetCurrentPrimaryInput(ConflictedInput);
				Control->SetCurrentPrimaryInput(OldInput);
				Control->SetSelectedPrimaryInput();

				return true;
			}

			const auto OtherInput = ControlToCheck->GetCurrentPrimaryInput();

			RebindInputMapping(ControlToCheck, ControlToCheck->GetCurrentPrimaryInput(), Control->GetCurrentPrimaryInput());
			RebindInputMapping(Control, Control->GetCurrentPrimaryInput(), ControlToCheck->GetCurrentPrimaryInput());

			ControlToCheck->SetCurrentPrimaryInput(Control->GetCurrentPrimaryInput());
			Control->SetCurrentPrimaryInput(OtherInput);
			Control->SetSelectedPrimaryInput();
			
			return true;
		}
	}

	return false;
}

bool UControlsMenu::IsGamepadInputKeyDuplicate(UInputKeyBinding* ControlToCheck, const FInputChord& InputToCheck)
{
	const auto Controls = GetAllControls();
	for (auto Control : Controls)
	{
		if (Control != ControlToCheck && Control->GetCurrentGamepadInput() == InputToCheck)
		{
			if (Control->GetInputName() == ControlToCheck->GetInputName())
			{
				const auto ConflictedInput = Control->GetCurrentGamepadInput();
				const auto OldInput = ControlToCheck->GetCurrentGamepadInput();

				RebindInputMapping(Control, Control->GetCurrentGamepadInput(), OldInput);
				RebindInputMapping(ControlToCheck, ControlToCheck->GetCurrentGamepadInput(), ConflictedInput);
				
				ControlToCheck->SetCurrentGamepadInput(ConflictedInput);
				Control->SetCurrentGamepadInput(OldInput);
				Control->SetSelectedGamepadInput();

				return true;
			}

			const auto OtherInput = ControlToCheck->GetCurrentGamepadInput();

			RebindInputMapping(ControlToCheck, ControlToCheck->GetCurrentGamepadInput(), Control->GetCurrentGamepadInput());
			RebindInputMapping(Control, Control->GetCurrentGamepadInput(), ControlToCheck->GetCurrentGamepadInput());

			ControlToCheck->SetCurrentGamepadInput(Control->GetCurrentGamepadInput());
			Control->SetCurrentGamepadInput(OtherInput);
			Control->SetSelectedGamepadInput();

			return true;
		}
	}

	return false;
}

