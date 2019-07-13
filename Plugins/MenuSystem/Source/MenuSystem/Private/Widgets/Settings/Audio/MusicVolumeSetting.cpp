// Copyright Ali El Saleh 2019

#include "Audio/MusicVolumeSetting.h"
#include "Kismet/GameplayStatics.h"
#include "Slider.h"
#include "TextBlock.h"

void UMusicVolumeSetting::Init()
{
	Super::Init();

	ValueWidget->SetText(FText::AsNumber(MusicVolume));
	VolumeSlider->SetValue(GetSliderValueAtVolume(MusicVolume));
	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, MusicSoundClass, GetSliderValueAtVolume(MusicVolume), 1.0f, 0.0f);
}

void UMusicVolumeSetting::ChangeMusicVolume(const float SliderValue)
{
	MusicVolume = int32(FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(float(0), float(100)), SliderValue));

	UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, MusicSoundClass, SliderValue, 1.0f, 0.0f);

	ValueWidget->SetText(FText::AsNumber(MusicVolume));

	SaveConfig(CPF_Config, *GGameUserSettingsIni);
}
