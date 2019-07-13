// Copyright Ali El Saleh 2019

#include "GraphicsSetting.h"
#include "ComboBoxString.h"
#include "WidgetTree.h"

void UGraphicsSetting::Init()
{
	Super::Init();

	AppliedChange = SelectedOption;

	DropDownList = Cast<UComboBoxString>(WidgetTree->FindWidget(FName("DropDown")));

	SetSelectedOption(DropDownList);
}

void UGraphicsSetting::Reset()
{
	SelectedOption = DefaultOption;
	AppliedChange = SelectedOption;

	ChangeGraphicsSetting(SelectedOption);
	SetSelectedOption(DropDownList);
}

void UGraphicsSetting::RevertChange()
{
	ChangeGraphicsSetting(AppliedChange);
	SetSelectedOption(DropDownList);
}

void UGraphicsSetting::ChangeGraphicsSetting(const int32 Index)
{
	QualityIndex = Index;
}

void UGraphicsSetting::SetSelectedOption(const int32 Index)
{
	switch (Index)
	{
	case 0:
		DropDownList->SetSelectedOption(Options[0]);
		break;
	case 1:
		DropDownList->SetSelectedOption(Options[1]);
		break;
	case 2:
		DropDownList->SetSelectedOption(Options[2]);
		break;
	case 3:
		DropDownList->SetSelectedOption(Options[3]);
		break;

	default:
		break;
	}
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
