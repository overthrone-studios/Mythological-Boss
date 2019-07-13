// Copyright Overthrone Studios 2019

#include "Audio/MasterVolumeSetting.h"
#include "TextBlock.h"
#include "Slider.h"
#include "Kismet/GameplayStatics.h"

void UMasterVolumeSetting::Init()
{
	Super::Init();

	ValueWidget->SetText(FText::AsNumber(MasterVolume));
	VolumeSlider->SetValue(GetSliderValueAtVolume(MasterVolume));
	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, MasterSoundClass, GetSliderValueAtVolume(MasterVolume), 1.0f, 0.0f);
}

void UMasterVolumeSetting::ChangeMasterVolume(const float SliderValue)
{
	MasterVolume = int32(FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(0.0f, 100.0f), SliderValue));

	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, MasterSoundClass, SliderValue, 1.0f, 0.0f);

	ValueWidget->SetText(FText::AsNumber(MasterVolume));

	SaveConfig(CPF_Config, *GGameUserSettingsIni);
}
