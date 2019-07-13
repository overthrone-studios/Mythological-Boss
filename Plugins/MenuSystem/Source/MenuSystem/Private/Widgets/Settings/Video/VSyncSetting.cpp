// Copyright Ali El Saleh 2019

#include "VSyncSetting.h"
#include "WidgetTree.h"
#include "ComboBoxString.h"

void UVSyncSetting::Init()
{
	Super::Init();

	DropDownList = Cast<UComboBoxString>(WidgetTree->FindWidget(FName("DropDown")));
	SetSelectedOption(DropDownList);
}

void UVSyncSetting::Apply()
{
	Super::Apply();

	GameUserSettings->bUseVSync = bVSyncEnabled;
	GameUserSettings->SetVSyncEnabled(bVSyncEnabled);
}

void UVSyncSetting::Reset()
{
	SelectedOption = DefaultOption;
	AppliedChange = SelectedOption;

	ChangeVSyncSetting(SelectedOption);
	SetSelectedOption(DropDownList);
}

void UVSyncSetting::RevertChange()
{
	ChangeVSyncSetting(AppliedChange);
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
		SelectedOption = Options[0];
		bVSyncEnabled = true;
	}
	else
	{
		SelectedOption = Options[1];
		bVSyncEnabled = false;
	}
}
