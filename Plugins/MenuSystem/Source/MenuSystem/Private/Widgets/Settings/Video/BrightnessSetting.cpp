// Copyright Ali El Saleh 2019

#include "BrightnessSetting.h"
#include "WidgetTree.h"
#include "Slider.h"
#include "TextBlock.h"

void UBrightnessSetting::Init()
{
	Super::Init();

	Slider = Cast<USlider>(WidgetTree->FindWidget(FName("BrightnessSlider")));
	Value = Cast<UTextBlock>(WidgetTree->FindWidget(FName("Value")));
}

void UBrightnessSetting::Apply()
{
	const FString Command = FString("r.Gamma ") + FString::SanitizeFloat(CurrentBrightness);

	//GetOwningPlayer()->ConsoleCommand(Command);
}

void UBrightnessSetting::Reset()
{
	CurrentBrightness = DefaultBrightness;

	ChangeBrightnessSetting(GetSliderValueAtBrightness(CurrentBrightness));
	Slider->SetValue(GetSliderValueAtDefaultBrightness());

	Apply();
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

	Value->SetText(FText::AsNumber(CurrentBrightness));

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
