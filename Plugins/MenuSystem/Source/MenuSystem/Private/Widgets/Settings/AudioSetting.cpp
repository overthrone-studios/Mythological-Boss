// Copyright Ali El Saleh 2019

#include "AudioSetting.h"

float UAudioSetting::GetSliderValueAtDefaultVolume()
{
	return FMath::GetMappedRangeValueClamped(FVector2D(float(0), float(100)), FVector2D(0.0f, 1.0f), DefaultVolume);
}

float UAudioSetting::GetSliderValueAtVolume(const int32 Value)
{
	return FMath::GetMappedRangeValueClamped(FVector2D(float(0), float(100)), FVector2D(0.0f, 1.0f), Value);
}
