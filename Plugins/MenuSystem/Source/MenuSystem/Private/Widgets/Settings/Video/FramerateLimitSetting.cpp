// Copyright Ali El Saleh 2019

#include "FramerateLimitSetting.h"
#include "WidgetTree.h"
#include "ComboBoxString.h"

void UFramerateLimitSetting::Init()
{
	Super::Init();

	DefaultOption = SelectedOption;
	PreviousSelectedOption = SelectedOption;

	DropDownList = Cast<UComboBoxString>(WidgetTree->FindWidget(FName("DropDown")));
}

void UFramerateLimitSetting::Apply()
{
	SelectedOption = PreviousSelectedOption;
	GameUserSettings->SetFrameRateLimit(NewFrameRateLimit);
}

void UFramerateLimitSetting::Reset()
{
	SelectedOption = DefaultOption;

	ChangeFrameRate(SelectedOption);
	SetSelectedOption(DropDownList);

	Apply();
}

bool UFramerateLimitSetting::HasChanged()
{
	return SelectedOption != PreviousSelectedOption;
}

void UFramerateLimitSetting::RevertChange()
{
	SelectedOption = PreviousSelectedOption;
	ChangeFrameRate(SelectedOption);
	SetSelectedOption(DropDownList);
}

void UFramerateLimitSetting::ChangeFrameRate(const FString& SelectedItem)
{
	for (auto Option : Options)
	{
		if (SelectedItem == Option)
		{
			if (Option == "Unlimited")
			{
				NewFrameRateLimit = 0.0f;
				break;
			}

			PreviousSelectedOption = SelectedOption;
			SelectedOption = Option;

			const FString SuffixToRemove = "FPS";
			Option.RemoveFromEnd(SuffixToRemove);
			Option.RemoveSpacesInline();

			NewFrameRateLimit = FCString::Atof(*Option);
			break;
		}
	}
}

void UFramerateLimitSetting::PopulateList(UComboBoxString* DropDownList)
{
	for (const auto& Item : Options)
		DropDownList->AddOption(Item);
}

void UFramerateLimitSetting::SetSelectedOption(UComboBoxString* DropDownList)
{
	DropDownList->SetSelectedOption(SelectedOption);
}
