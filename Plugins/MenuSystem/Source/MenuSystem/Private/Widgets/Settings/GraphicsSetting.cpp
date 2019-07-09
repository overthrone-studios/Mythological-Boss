// Copyright Ali El Saleh 2019

#include "GraphicsSetting.h"
#include "ComboBoxString.h"
#include "WidgetTree.h"

void UGraphicsSetting::Init()
{
	Super::Init();

	DefaultOption = SelectedOption;
	AppliedChange = SelectedOption;

	DropDownList = Cast<UComboBoxString>(WidgetTree->FindWidget(FName("DropDown")));
}

void UGraphicsSetting::Reset()
{
	SelectedOption = DefaultOption;

	ChangeGraphicsSetting(SelectedOption);
	SetSelectedOption(DropDownList);
}

void UGraphicsSetting::RevertChange()
{
	ChangeGraphicsSetting(AppliedChange);
	SetSelectedOption(DropDownList);
}

void UGraphicsSetting::ChangeGraphicsSetting(const FString& SelectedItem)
{
	int32 Index = 0;
	for (const auto& Option : Options)
	{
		if (SelectedItem == Option)
		{
			SelectedOption = Option;
			QualityIndex = Index;
			break;
		}

		Index++;
	}
}

void UGraphicsSetting::PopulateList(UComboBoxString* DropDownList)
{
	for (const auto& Option : Options)
	{
		DropDownList->AddOption(Option);
	}
}

void UGraphicsSetting::SetSelectedOption(UComboBoxString* DropDownList)
{
	DropDownList->SetSelectedOption(SelectedOption);
}
