// Copyright Ali El Saleh 2019

#include "ControlsMenu.h"
#include "MenuHUD.h"
#include "WidgetTree.h"
#include "MenuSetting.h"
#include "OptionsMenu.h"
#include "InputKeyBinding.h"
#include "GameFramework/InputSettings.h"
#include "ConfigCacheIni.h"
#include "ComboBoxString.h"
#include "InvertSetting.h"

void UControlsMenu::Init()
{
	Super::Init();

	ResetWarningBox = Cast<UUserWidget>(WidgetTree->FindWidget(FName("ResetWarningBox")));
	MouseInvertXSetting = Cast<UInvertSetting>(WidgetTree->FindWidget(FName("MouseInvertX")));
	MouseInvertYSetting = Cast<UInvertSetting>(WidgetTree->FindWidget(FName("MouseInvertY")));
	GamepadInvertXSetting = Cast<UInvertSetting>(WidgetTree->FindWidget(FName("GamepadInvertX")));
	GamepadInvertYSetting = Cast<UInvertSetting>(WidgetTree->FindWidget(FName("GamepadInvertY")));

	InitializeButtons();

	InitializeSettings();

	InitializeControls();

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
	Input->SaveConfig();
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

	// Save to input config file (DefaultInput.ini)
	Input->SaveKeyMappings();

	// Update in Project Settings -> Engine -> Input
	Input->ForceRebuildKeymaps();
}

void UControlsMenu::Reset()
{
	ResetKeyBindings();
}

void UControlsMenu::HideWidgets()
{
	ResetWarningBox->SetVisibility(ESlateVisibility::Hidden);
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

