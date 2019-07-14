// Copyright Ali El Saleh 2019

#include "VideoSetting.h"
#include "Log.h"

void UVideoSetting::Init()
{
	Super::Init();

	LoadConfig(GetClass(), *GGameUserSettingsIni);

	if (SelectedOption.IsEmpty())
	{
		SelectedOption = DefaultOption;
	}

	AppliedChange = SelectedOption;
}

void UVideoSetting::Apply()
{
	AppliedChange = SelectedOption;

	SaveConfig(CPF_Config, *GGameUserSettingsIni);
}

bool UVideoSetting::HasChanged()
{
	return SelectedOption != AppliedChange;
}

bool UVideoSetting::IsDefault()
{
	return SelectedOption == DefaultOption;
}
