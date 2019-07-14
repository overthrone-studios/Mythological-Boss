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
	DropDownList->SetSelectedOption(SelectedOption);
}

void UDisplayModeSetting::ChangeDisplayMode(const FString& SelectedItem)
{
	if (!bInitialized)
		return;

	SelectedOption = SelectedItem;

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

	SaveConfig(CPF_Config, *GGameUserSettingsIni);
}

void UDisplayModeSetting::Init()
{
	Super::Init();

	AppliedDisplayMode = DisplayMode;

	DropDownList = Cast<UComboBoxString>(WidgetTree->FindWidget(FName("DropDown")));
	SetSelectedOption(DropDownList);

	bInitialized = true;
	ChangeDisplayMode(SelectedOption);
}

void UDisplayModeSetting::Apply()
{
	Super::Apply();

	AppliedDisplayMode = DisplayMode;

	GameUserSettings->SetFullscreenMode(DisplayMode);
}

void UDisplayModeSetting::Reset()
{
	DisplayMode = DefaultDisplayMode;
	AppliedDisplayMode = DisplayMode;
	SelectedOption = DefaultOption;
	AppliedChange = SelectedOption;

	ChangeDisplayMode(SelectedOption);
	SetSelectedOption(DropDownList);
}

bool UDisplayModeSetting::IsAffectedByReset()
{
	return false;
}

bool UDisplayModeSetting::HasChanged()
{
	if (DisplayMode == AppliedDisplayMode)
		return false;
	
	return true;
}

void UDisplayModeSetting::RevertChange()
{
	ChangeDisplayMode(AppliedChange);
	SetSelectedOption(DropDownList);
}
