// Copyright Ali El Saleh 2019

#include "OverallQualitySetting.h"
#include "MenuBase.h"
#include "GraphicsSetting.h"
#include "ComboBoxString.h"
#include "WidgetTree.h"
#include "Log.h"

void UOverallQualitySetting::Init()
{
	Super::Init();

	DropDownList = Cast<UComboBoxString>(WidgetTree->FindWidget(FName("DropDown")));

	SetSelectedOption(DropDownList);

	// Disable all graphics settings
	if (SelectedOption == Options[4])
	{
		const auto Settings = GetAllGraphicsSettings();
		for (auto Setting : Settings)
		{
			Setting->SetIsEnabled(true);
		}
	}
	else
	{
		const auto Settings = GetAllGraphicsSettings();
		for (auto Setting : Settings)
		{
			Setting->SetIsEnabled(false);
		}
	}
}

void UOverallQualitySetting::Apply()
{
	Super::Apply();

	// Don't apply overall settings when in custom mode
	if (SelectedOption == Options[4])
		return;

	TArray<UGraphicsSetting*> GraphicsSettings = GetAllGraphicsSettings();

	for (auto Setting : GraphicsSettings)
	{
		Setting->ChangeGraphicsSetting(OverallQualityIndex);
		Setting->SetSelectedOption(OverallQualityIndex);
		Setting->Apply();
	}
}

void UOverallQualitySetting::RevertChange()
{
	ChangeAllQualitySettings(AppliedChange);
	SetSelectedOption(DropDownList);
}

void UOverallQualitySetting::Reset()
{
	AppliedChange = DefaultOption;

	ChangeAllQualitySettings(DefaultOption);
	SetSelectedOption(DropDownList);
}

void UOverallQualitySetting::ChangeAllQualitySettings(const FString& SelectedItem)
{
	SelectedOption = SelectedItem;

	// Disable all graphics settings
	const auto Settings = GetAllGraphicsSettings();
	for (auto Setting : Settings)
		Setting->SetIsEnabled(false);

	if (SelectedItem == Options[0])
		OverallQualityIndex = 0;
	else if (SelectedItem == Options[1])
		OverallQualityIndex = 1;
	else if (SelectedItem == Options[2])
		OverallQualityIndex = 2;
	else if (SelectedItem == Options[3])
		OverallQualityIndex = 3;
	else if (SelectedItem == Options[4])
	{
		AppliedChange = SelectedOption;

		// Enable all graphics settings
		for (auto Setting : Settings)
			Setting->SetIsEnabled(true);
		
		SaveConfig(CPF_Config, *GGameUserSettingsIni);
	}
}

void UOverallQualitySetting::PopulateList(UComboBoxString* DropDownList)
{
	for (const auto& Option : Options)
		DropDownList->AddOption(Option);
}

void UOverallQualitySetting::SetSelectedOption(UComboBoxString* DropDownList)
{
	DropDownList->SetSelectedOption(SelectedOption);
}

TArray<UGraphicsSetting*> UOverallQualitySetting::GetAllGraphicsSettings()
{
	TArray<UGraphicsSetting*> FoundSettings;
	TArray<UWidget*> WidgetsInMenu;

	if (!Menu)
		return TArray<UGraphicsSetting*>();

	Menu->WidgetTree->GetAllWidgets(WidgetsInMenu);

	for (auto Widget : WidgetsInMenu)
	{
		if (Widget->IsA(UGraphicsSetting::StaticClass()))
			FoundSettings.Add(Cast<UGraphicsSetting>(Widget));
	}

	return FoundSettings;
}
