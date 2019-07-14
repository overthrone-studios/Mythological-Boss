// Copyright Ali El Saleh 2019

#include "ResolutionSetting.h"
#include "WidgetTree.h"
#include "ComboBoxString.h"
#include "Log.h"

void UResolutionSetting::ChangeResolution(const FString& SelectedItem)
{
	if (!bInitialized)
		return;

	SelectedOption = SelectedItem;

	// Get the selected resolution string and split it up
	FString XRes;
	FString YRes;
	SelectedItem.Split(FString("x"), &XRes, &YRes);

	// Once split, convert strings into integers
	const int32 X = FCString::Atoi(*XRes);
	const int32 Y = FCString::Atoi(*YRes);

	// Set the new resolution
	NewResolution = FIntPoint(X, Y);
}

void UResolutionSetting::PopulateResolutionList(UComboBoxString* DropDownList)
{
	// Get every resolution supported
	RHIGetAvailableResolutions(ScreenResolutions, true);

	// Populate the combo box
	for (int32 i = ScreenResolutions.Num() - 1; i >= 0; i--)
	{
		FString Option = FString::FromInt(ScreenResolutions[i].Width) + "x" + FString::FromInt(ScreenResolutions[i].Height);

		DropDownList->AddOption(Option);
		
		if (DropDownList->GetOptionCount() == MaxResolutions)
			break;
	}
}

void UResolutionSetting::SetSelectedOption(UComboBoxString* DropDownList)
{
	// Set the default resolution as the selected option
	DropDownList->SetSelectedOption(SelectedOption);
}

TArray<FString> UResolutionSetting::GetAllSupportedResolutions()
{
	TArray<FString> Resolutions;

	for (int32 i = ScreenResolutions.Num() - 1; i >= 0; i--)
	{
		FString Option = FString::FromInt(ScreenResolutions[i].Width) + "x" + FString::FromInt(ScreenResolutions[i].Height);

		Resolutions.Add(Option);
	}

	return Resolutions;
}

void UResolutionSetting::Init()
{
	Super::Init();

	DefaultOption = FString::FromInt(GameUserSettings->GetDesktopResolution().X) + FString("x") + FString::FromInt(GameUserSettings->GetDesktopResolution().Y);

#if !WITH_EDITOR
	if (SelectedOption != DefaultOption)
		SelectedOption = DefaultOption;
#endif

	DropDownList = Cast<UComboBoxString>(WidgetTree->FindWidget(FName("DropDown")));
	SetSelectedOption(DropDownList);

	bInitialized = true;
	ChangeResolution(SelectedOption);
}

void UResolutionSetting::Apply()
{
	Super::Apply();

	GameUserSettings->SetScreenResolution(NewResolution);
}

void UResolutionSetting::Reset()
{
	AppliedChange = SelectedOption;

	ChangeResolution(SelectedOption);
	SetSelectedOption(DropDownList);
}

bool UResolutionSetting::IsAffectedByReset()
{
	return false;
}

void UResolutionSetting::RevertChange()
{
	ChangeResolution(AppliedChange);
	SetSelectedOption(DropDownList);
}
