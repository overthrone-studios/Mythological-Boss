// Copyright Ali El Saleh 2019

#include "Audio/MuteAllSetting.h"
#include "WidgetTree.h"
#include "CheckBox.h"
#include "Sound/SoundClass.h"

void UMuteAllSetting::Init()
{
	Super::Init();

	MuteToggle = Cast<UCheckBox>(WidgetTree->FindWidget(FName("MuteToggle")));

	ToggleMute(bIsMuted);
	MuteToggle->SetIsChecked(bIsMuted);
}

void UMuteAllSetting::ToggleMute(const bool bShouldMute)
{
	if (bShouldMute)
	{
		MasterSoundClass->Properties.Volume = 0.0f;
		MusicSoundClass->Properties.Volume = 0.0f;
		SFXSoundClass->Properties.Volume = 0.0f;
		UISoundClass->Properties.Volume = 0.0f;
		EnvironmentSoundClass->Properties.Volume = 0.0f;
	}
	else
	{
		MasterSoundClass->Properties.Volume = 1.0f;
		MusicSoundClass->Properties.Volume = 1.0f;
		SFXSoundClass->Properties.Volume = 1.0f;
		UISoundClass->Properties.Volume = 1.0f;
		EnvironmentSoundClass->Properties.Volume = 1.0f;
	}

	SaveConfig(CPF_Config, *GGameUserSettingsIni);
}
