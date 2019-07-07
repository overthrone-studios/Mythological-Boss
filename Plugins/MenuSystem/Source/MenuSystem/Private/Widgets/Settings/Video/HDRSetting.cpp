// Copyright Ali El Saleh 2019

#include "HDRSetting.h"
#include "ComboBoxString.h"

void UHDRSetting::Init()
{
	Super::Init();

	if (GameUserSettings->SupportsHDRDisplayOutput())
		SelectedOption = Options[1];
	else
		SelectedOption = Options[0];
}

void UHDRSetting::Apply()
{
	if (GameUserSettings->SupportsHDRDisplayOutput())
		GameUserSettings->EnableHDRDisplayOutput(bHDREnabled);
}

void UHDRSetting::ChangeHDRSetting(const FString& SelectedItem)
{
	if (SelectedItem == Options[0])
	{
		SelectedOption = Options[0];
		bHDREnabled = true;
	}
	else
	{
		SelectedOption = Options[1];
		bHDREnabled = false;
	}
}

void UHDRSetting::PopulateList(UComboBoxString* DropDownList)
{
	for (const auto& Option : Options)
	{
		DropDownList->AddOption(Option);
	}
}

void UHDRSetting::SetSelectedOption(UComboBoxString* DropDownList)
{
	DropDownList->SetSelectedOption(SelectedOption);
}

