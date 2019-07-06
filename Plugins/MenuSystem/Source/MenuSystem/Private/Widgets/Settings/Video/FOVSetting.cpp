// Copyright Ali El Saleh 2019

#include "FOVSetting.h"
#include "Kismet/GameplayStatics.h"

void UFOVSetting::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (DefaultFOV < MinFOV)
		DefaultFOV = MinFOV;

	if (DefaultFOV > MaxFOV)
		DefaultFOV = MaxFOV;

	CurrentFOV = DefaultFOV;
}

void UFOVSetting::ChangeFOVSetting(const float SliderValue)
{
	CurrentFOV = int32(FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(float(MinFOV), float(MaxFOV)), SliderValue));

	if (bApplyOnChange)
		Apply();
}

float UFOVSetting::GetSliderValueAtDefaultFOV()
{
	return FMath::GetMappedRangeValueClamped(FVector2D(float(MinFOV), float(MaxFOV)), FVector2D(0.0f, 1.0f), DefaultFOV);
}

float UFOVSetting::GetSliderValueAtFOV(const int32 Value)
{
	return FMath::GetMappedRangeValueClamped(FVector2D(float(MinFOV), float(MaxFOV)), FVector2D(0.0f, 1.0f), Value);
}

void UFOVSetting::Apply()
{
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->SetFOV(CurrentFOV);
}
