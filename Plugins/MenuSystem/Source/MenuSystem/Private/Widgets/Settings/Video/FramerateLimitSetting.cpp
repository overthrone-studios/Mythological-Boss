// Copyright Ali El Saleh 2019

#include "FramerateLimitSetting.h"
#include "WidgetTree.h"
#include "ComboBoxString.h"

void UFramerateLimitSetting::Init()
{
	Super::Init();

	DropDownList = Cast<UComboBoxString>(WidgetTree->FindWidget(FName("DropDown")));
	SetSelectedOption(DropDownList);
}

void UFramerateLimitSetting::Apply()
{
	Super::Apply();
	
	GameUserSettings->SetFrameRateLimit(NewFrameRateLimit);
}

void UFramerateLimitSetting::Reset()
{
	SelectedOption = DefaultOption;
	AppliedChange = SelectedOption;

	ChangeFrameRate(SelectedOption);
	SetSelectedOption(DropDownList);
}

void UFramerateLimitSetting::RevertChange()
{
	ChangeFrameRate(AppliedChange);
	SetSelectedOption(DropDownList);
}

void UFramerateLimitSetting::ChangeFrameRate(const FString& SelectedItem)
{
	for (auto Option : Options)
	{
		if (SelectedItem == Option)
		{
			SelectedOption = Option;

			if (Option == "Unlimited")
			{
				NewFrameRateLimit = 0.0f;
				break;
			}

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
