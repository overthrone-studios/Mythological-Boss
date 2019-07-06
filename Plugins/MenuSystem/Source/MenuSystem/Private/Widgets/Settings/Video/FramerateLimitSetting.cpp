// Copyright Ali El Saleh 2019

#include "FramerateLimitSetting.h"
#include "ComboBoxString.h"

void UFramerateLimitSetting::Apply()
{
	GameUserSettings->SetFrameRateLimit(NewFrameRateLimit);
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
