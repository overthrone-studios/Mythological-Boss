// Copyright Ali El Saleh 2019

#include "Audio/EnvironmentVolumeSetting.h"
#include "Kismet/GameplayStatics.h"
#include "Slider.h"
#include "TextBlock.h"

void UEnvironmentVolumeSetting::Init()
{
	Super::Init();
	
	ValueWidget->SetText(FText::AsNumber(EnvironmentVolume));
	VolumeSlider->SetValue(GetSliderValueAtVolume(EnvironmentVolume));
	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, EnvironmentSoundClass, GetSliderValueAtVolume(EnvironmentVolume), 1.0f, 0.0f);
}

void UEnvironmentVolumeSetting::ChangeEnvironmentVolume(const float SliderValue)
{
	EnvironmentVolume = int32(FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(float(0), float(100)), SliderValue));

	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, EnvironmentSoundClass, SliderValue, 1.0f, 0.0f);

	ValueWidget->SetText(FText::AsNumber(EnvironmentVolume));

	SaveConfig(CPF_Config, *GGameUserSettingsIni);
}
