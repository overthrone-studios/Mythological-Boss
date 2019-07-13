// Copyright Ali El Saleh 2019

#include "AudioSetting.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Kismet/GameplayStatics.h"
#include "WidgetTree.h"
#include "TextBlock.h"
#include "Slider.h"
#include "CheckBox.h"

UAudioSetting::UAudioSetting()
{
	MasterSoundClass = Cast<USoundClass>(StaticLoadObject(USoundClass::StaticClass(), nullptr, TEXT("SoundClass'/Game/Sounds/Classes/MasterSoundClass.MasterSoundClass'")));
	MusicSoundClass = Cast<USoundClass>(StaticLoadObject(USoundClass::StaticClass(), nullptr, TEXT("SoundClass'/Game/Sounds/Classes/MusicSoundClass.MusicSoundClass'")));
	SFXSoundClass = Cast<USoundClass>(StaticLoadObject(USoundClass::StaticClass(), nullptr, TEXT("SoundClass'/Game/Sounds/Classes/SFXSoundClass.SFXSoundClass'")));
	UISoundClass = Cast<USoundClass>(StaticLoadObject(USoundClass::StaticClass(), nullptr, TEXT("SoundClass'/Game/Sounds/Classes/UISoundClass.UISoundClass'")));
	EnvironmentSoundClass = Cast<USoundClass>(StaticLoadObject(USoundClass::StaticClass(), nullptr, TEXT("SoundClass'/Game/Sounds/Classes/EnvironmentSoundClass.EnvironmentSoundClass'")));

	MasterMix = Cast<USoundMix>(StaticLoadObject(USoundMix::StaticClass(), nullptr, TEXT("SoundMix'/Game/Sounds/Mixes/MasterSoundMix.MasterSoundMix'")));

	LoadConfig(GetClass(), *GGameUserSettingsIni);
}

void UAudioSetting::Init()
{
	Super::Init();

	VolumeSlider = Cast<USlider>(WidgetTree->FindWidget(FName("VolumeSlider")));
	ValueWidget = Cast<UTextBlock>(WidgetTree->FindWidget(FName("Value")));

	LoadConfig(GetClass(), *GGameUserSettingsIni);

	UGameplayStatics::SetBaseSoundMix(this, MasterMix);
}

float UAudioSetting::GetSliderValueAtDefaultVolume()
{
	return FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 100.0f), FVector2D(0.0f, 1.0f), DefaultVolume);
}

float UAudioSetting::GetSliderValueAtVolume(const float Value)
{
	return FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 100.0f), FVector2D(0.0f, 1.0f), Value);
}
