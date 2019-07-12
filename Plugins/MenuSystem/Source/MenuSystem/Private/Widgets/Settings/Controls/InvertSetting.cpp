// Copyright Ali El Saleh 2019

#include "InvertSetting.h"
#include "WidgetTree.h"
#include "ComboBoxString.h"

void UInvertSetting::Init()
{
	Super::Init();

	DefaultOption = SelectedOption;

	DropDownList = Cast<UComboBoxString>(WidgetTree->FindWidget(FName("DropDown")));

	SetSelectedOption(DropDownList);
}

void UInvertSetting::Reset()
{
	SelectedOption = DefaultOption;

	ChangeMouseInvert(SelectedOption);
	SetSelectedOption(DropDownList);
}

bool UInvertSetting::IsDefault()
{
	return SelectedOption == DefaultOption;
}

void UInvertSetting::ChangeMouseInvert(const FString& SelectedItem)
{
	if (SelectedItem == Options[0])
	{	
		SelectedOption = Options[0];
		bIsInverted = true;
	}
	else
	{
		SelectedOption = Options[1];
		bIsInverted = false;
	}
}

void UInvertSetting::PopulateList(UComboBoxString* DropDownList)
{
	for (const FString& Option : Options)
		DropDownList->AddOption(Option);
}

void UInvertSetting::SetSelectedOption(UComboBoxString* DropDownList)
{
	DropDownList->SetSelectedOption(SelectedOption);
}
