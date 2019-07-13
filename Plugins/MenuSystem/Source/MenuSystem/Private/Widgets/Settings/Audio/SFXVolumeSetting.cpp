// Copyright Ali El Saleh 2019

#include "Audio/SFXVolumeSetting.h"
#include "Kismet/GameplayStatics.h"
#include "Slider.h"
#include "TextBlock.h"

void USFXVolumeSetting::Init()
{
	Super::Init();
	
	ValueWidget->SetText(FText::AsNumber(SFXVolume));
	VolumeSlider->SetValue(GetSliderValueAtVolume(SFXVolume));
	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, SFXSoundClass, GetSliderValueAtVolume(SFXVolume), 1.0f, 0.0f);
}

void USFXVolumeSetting::ChangeSFXVolume(const float SliderValue)
{
	SFXVolume = int32(FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(float(0), float(100)), SliderValue));

	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, SFXSoundClass, SliderValue, 1.0f, 0.0f);

	ValueWidget->SetText(FText::AsNumber(SFXVolume));

	SaveConfig(CPF_Config, *GGameUserSettingsIni);
}
