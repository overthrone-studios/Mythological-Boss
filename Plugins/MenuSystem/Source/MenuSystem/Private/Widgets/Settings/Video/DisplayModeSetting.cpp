// Copyright Ali El Saleh 2019

#include "DisplayModeSetting.h"
#include "ComboBoxString.h"

void UDisplayModeSetting::PopulateDisplayModeList(UComboBoxString* DropDownList)
{
	DropDownList->AddOption(FString("Windowed"));
	DropDownList->AddOption(FString("Windowed Fullscreen"));
	DropDownList->AddOption(FString("Fullscreen"));
}

void UDisplayModeSetting::SetSelectedOption(UComboBoxString* DropDownList)
{
	switch (DisplayMode)
	{
	case EWindowMode::Windowed:
		DropDownList->SetSelectedOption(FString("Windowed"));
	break;

	case EWindowMode::WindowedFullscreen:
		DropDownList->SetSelectedOption(FString("Windowed Fullscreen"));
	break;

	case EWindowMode::Fullscreen:
		DropDownList->SetSelectedOption(FString("Fullscreen"));
	break;

	default:
	break;
	}
}

void UDisplayModeSetting::ChangeDisplayMode(const FString& SelectedItem)
{
	if (SelectedItem == "Windowed")
	{
		DisplayMode = EWindowMode::Windowed;
	}
	else if (SelectedItem == "Windowed Fullscreen")
	{
		DisplayMode = EWindowMode::WindowedFullscreen;
	}
	else if (SelectedItem == "Fullscreen")
	{
		DisplayMode = EWindowMode::Fullscreen;
	}
}

void UDisplayModeSetting::Apply()
{
	GameUserSettings->SetFullscreenMode(DisplayMode);
}
