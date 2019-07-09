// Copyright Ali El Saleh 2019

#include "VSyncSetting.h"
#include "WidgetTree.h"
#include "ComboBoxString.h"

void UVSyncSetting::Init()
{
	Super::Init();

	DefaultOption = SelectedOption;
	PreviousSelectedOption = SelectedOption;

	DropDownList = Cast<UComboBoxString>(WidgetTree->FindWidget(FName("DropDown")));
}

void UVSyncSetting::Apply()
{
	GameUserSettings->bUseVSync = bVSyncEnabled;
	GameUserSettings->SetVSyncEnabled(bVSyncEnabled);
	SelectedOption = PreviousSelectedOption;
}

void UVSyncSetting::Reset()
{
	SelectedOption = DefaultOption;

	ChangeVSyncSetting(SelectedOption);
	SetSelectedOption(DropDownList);

	Apply();
}

bool UVSyncSetting::HasChanged()
{
	return SelectedOption != PreviousSelectedOption;
}

void UVSyncSetting::RevertChange()
{
	SelectedOption = PreviousSelectedOption;
	ChangeVSyncSetting(SelectedOption);
	SetSelectedOption(DropDownList);
}

void UVSyncSetting::PopulateList(UComboBoxString* DropDownList)
{
	for (const auto& Option : Options)
	{
		DropDownList->AddOption(Option);
	}
}

void UVSyncSetting::SetSelectedOption(UComboBoxString* DropDownList)
{
	DropDownList->SetSelectedOption(SelectedOption);
}

void UVSyncSetting::ChangeVSyncSetting(const FString& Selection)
{
	if (Selection == Options[0])
	{	
		PreviousSelectedOption = SelectedOption;
		SelectedOption = Options[0];
		bVSyncEnabled = true;
	}
	else
	{
		PreviousSelectedOption = SelectedOption;
		SelectedOption = Options[1];
		bVSyncEnabled = false;
	}
}
