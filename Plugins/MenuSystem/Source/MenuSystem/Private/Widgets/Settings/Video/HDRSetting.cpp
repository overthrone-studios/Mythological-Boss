// Copyright Ali El Saleh 2019

#include "HDRSetting.h"
#include "WidgetTree.h"
#include "ComboBoxString.h"

void UHDRSetting::Init()
{
	Super::Init();

	DefaultOption = SelectedOption;
	PreviousSelectedOption = SelectedOption;

	DropDownList = Cast<UComboBoxString>(WidgetTree->FindWidget(FName("DropDown")));

	if (GameUserSettings->SupportsHDRDisplayOutput())
		SelectedOption = Options[1];
	else
		SelectedOption = Options[0];
}

void UHDRSetting::Apply()
{
	if (GameUserSettings->SupportsHDRDisplayOutput())
		GameUserSettings->EnableHDRDisplayOutput(bHDREnabled);

	SelectedOption = PreviousSelectedOption;
}

void UHDRSetting::Reset()
{
	SelectedOption = DefaultOption;

	ChangeHDRSetting(SelectedOption);
	SetSelectedOption(DropDownList);

	Apply();
}

bool UHDRSetting::HasChanged()
{
	return SelectedOption != PreviousSelectedOption;
}

void UHDRSetting::RevertChange()
{
	SelectedOption = PreviousSelectedOption;
	ChangeHDRSetting(SelectedOption);
	SetSelectedOption(DropDownList);
}

void UHDRSetting::ChangeHDRSetting(const FString& SelectedItem)
{
	if (SelectedItem == Options[0])
	{
		PreviousSelectedOption = SelectedOption;
		SelectedOption = Options[0];
		bHDREnabled = true;
	}
	else
	{
		PreviousSelectedOption = SelectedOption;
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

