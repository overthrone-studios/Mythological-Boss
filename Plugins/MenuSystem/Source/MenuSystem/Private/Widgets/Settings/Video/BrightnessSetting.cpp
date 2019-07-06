// Copyright Ali El Saleh 2019

#include "BrightnessSetting.h"

void UBrightnessSetting::Apply()
{
	const FString Command = FString("r.Gamma ") + FString::SanitizeFloat(CurrentBrightness);

	//GetOwningPlayer()->ConsoleCommand(Command);
}

void UBrightnessSetting::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (DefaultBrightness < MinBrightness)
		DefaultBrightness = MinBrightness;

	if (DefaultBrightness > MaxBrightness)
		DefaultBrightness = MaxBrightness;

	CurrentBrightness = DefaultBrightness;
}

void UBrightnessSetting::ChangeBrightnessSetting(const float SliderValue)
{
	CurrentBrightness = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(MinBrightness, MaxBrightness), SliderValue);

	if (bApplyOnChange)
		Apply();
}

float UBrightnessSetting::GetSliderValueAtDefaultBrightness()
{
	return FMath::GetMappedRangeValueClamped(FVector2D(MinBrightness, MaxBrightness), FVector2D(0.0f, 1.0f), DefaultBrightness);
}

float UBrightnessSetting::GetSliderValueAtBrightness(const float Value)
{
	return FMath::GetMappedRangeValueClamped(FVector2D(MinBrightness, MaxBrightness), FVector2D(0.0f, 1.0f), Value);
}
