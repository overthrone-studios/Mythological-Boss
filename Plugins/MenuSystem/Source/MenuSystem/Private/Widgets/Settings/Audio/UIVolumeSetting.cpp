// Copyright Ali El Saleh 2019

#include "Audio/UIVolumeSetting.h"
#include "Kismet/GameplayStatics.h"
#include "Slider.h"
#include "TextBlock.h"

void UUIVolumeSetting::Init()
{
	Super::Init();
	
	ValueWidget->SetText(FText::AsNumber(UIVolume));
	VolumeSlider->SetValue(GetSliderValueAtVolume(UIVolume));
	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, UISoundClass, GetSliderValueAtVolume(UIVolume), 1.0f, 0.0f);
}

void UUIVolumeSetting::ChangeUIVolume(const float SliderValue)
{
	UIVolume = int32(FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(0.0f, 100.0f), SliderValue));

	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, UISoundClass, SliderValue, 1.0f, 0.0f);

	ValueWidget->SetText(FText::AsNumber(UIVolume));

	SaveConfig(CPF_Config, *GGameUserSettingsIni);
}
