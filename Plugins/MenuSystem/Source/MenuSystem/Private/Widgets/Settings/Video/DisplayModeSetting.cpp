// Copyright Ali El Saleh 2019

#include "DisplayModeSetting.h"
#include "WidgetTree.h"
#include "ComboBoxString.h"
#include "Log.h"

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
		SelectedOption = "Windowed";
		DropDownList->SetSelectedOption(SelectedOption);
	break;

	case EWindowMode::WindowedFullscreen:
		SelectedOption = "Windowed Fullscreen";
		DropDownList->SetSelectedOption(SelectedOption);
	break;

	case EWindowMode::Fullscreen:
		SelectedOption = "Fullscreen";
		DropDownList->SetSelectedOption(SelectedOption);
	break;

	default:
	break;
	}
}

void UDisplayModeSetting::ChangeDisplayMode(const FString& SelectedItem)
{
	PreviousDisplayMode = DisplayMode;

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

void UDisplayModeSetting::Init()
{
	Super::Init();

	DefaultDisplayMode = DisplayMode;
	PreviousDisplayMode = DisplayMode;

	DropDownList = Cast<UComboBoxString>(WidgetTree->FindWidget(FName("DropDown")));
}

void UDisplayModeSetting::Apply()
{
	ULog::LogDebugMessage(INFO, FString("DisplayMode: ") + FString::FromInt(DisplayMode), true);
	ULog::LogDebugMessage(INFO, FString("PreviousDisplayMode: ") + FString::FromInt(PreviousDisplayMode), true);

	GameUserSettings->SetFullscreenMode(DisplayMode);
	DisplayMode = PreviousDisplayMode;
}

bool UDisplayModeSetting::HasChanged()
{
	if (DisplayMode == PreviousDisplayMode)
		return false;
	
	return true;
}

void UDisplayModeSetting::RevertChange()
{
	DisplayMode = PreviousDisplayMode;
	SetSelectedOption(DropDownList);

	ChangeDisplayMode(SelectedOption);
}
