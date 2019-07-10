// Copyright Ali El Saleh 2019

#include "Controls/MouseInvertSetting.h"
#include "WidgetTree.h"
#include "ComboBoxString.h"

void UMouseInvertSetting::Init()
{
	Super::Init();

	DefaultOption = SelectedOption;

	DropDownList = Cast<UComboBoxString>(WidgetTree->FindWidget(FName("DropDown")));
}

void UMouseInvertSetting::Apply()
{
	
}

void UMouseInvertSetting::Reset()
{
	SelectedOption = DefaultOption;

	ChangeMouseInvert(SelectedOption);
	SetSelectedOption(DropDownList);
}

bool UMouseInvertSetting::IsDefault()
{
	return SelectedOption == DefaultOption;
}

void UMouseInvertSetting::ChangeMouseInvert(const FString& SelectedItem)
{
	if (SelectedItem == Options[0])
	{	
		SelectedOption = Options[0];
		// Enable
	}
	else
	{
		SelectedOption = Options[1];
		// Disable
	}
}

void UMouseInvertSetting::PopulateList(UComboBoxString* DropDownList)
{
	for (const FString& Option : Options)
		DropDownList->AddOption(Option);
}

void UMouseInvertSetting::SetSelectedOption(UComboBoxString* DropDownList)
{
	DropDownList->SetSelectedOption(SelectedOption);
}
