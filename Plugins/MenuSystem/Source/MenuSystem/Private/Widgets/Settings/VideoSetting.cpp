// Copyright Ali El Saleh 2019

#include "VideoSetting.h"

void UVideoSetting::Init()
{
	Super::Init();

	DefaultOption = SelectedOption;
	AppliedChange = SelectedOption;
}

void UVideoSetting::Apply()
{
	AppliedChange = SelectedOption;
}

bool UVideoSetting::HasChanged()
{
	return SelectedOption != AppliedChange;
}

bool UVideoSetting::IsDefault()
{
	return SelectedOption == DefaultOption;
}
