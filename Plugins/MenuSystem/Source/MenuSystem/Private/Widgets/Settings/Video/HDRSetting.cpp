// Copyright Ali El Saleh 2019

#include "HDRSetting.h"
#include "WidgetTree.h"
#include "ComboBoxString.h"

void UHDRSetting::Init()
{
	Super::Init();

	DefaultOption = SelectedOption;
	AppliedChange = SelectedOption;

	DropDownList = Cast<UComboBoxString>(WidgetTree->FindWidget(FName("DropDown")));

	if (GameUserSettings->SupportsHDRDisplayOutput())
	{
		SelectedOption = Options[1];
		AppliedChange = SelectedOption;
	}
	else
	{
		SelectedOption = Options[0];
		AppliedChange = SelectedOption;
	}
}

void UHDRSetting::Apply()
{
	Super::Apply();

	if (GameUserSettings->SupportsHDRDisplayOutput())
		GameUserSettings->EnableHDRDisplayOutput(bHDREnabled);
}

void UHDRSetting::Reset()
{
	SelectedOption = DefaultOption;

	ChangeHDRSetting(SelectedOption);
	SetSelectedOption(DropDownList);
}

void UHDRSetting::RevertChange()
{
	ChangeHDRSetting(AppliedChange);
	SetSelectedOption(DropDownList);
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

