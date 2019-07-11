// Copyright Ali El Saleh 2019

#include "Controls/SensitivitySetting.h"
#include "WidgetTree.h"
#include "TextBlock.h"
#include "Slider.h"
#include "Kismet/KismetMathLibrary.h"

void USensitivitySetting::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (DefaultSensitivity < MinSensitivity)
		DefaultSensitivity = MinSensitivity;

	if (DefaultSensitivity > MaxSensitivity)
		DefaultSensitivity = MaxSensitivity;

	StepSize = 1/(MaxSensitivity-MinSensitivity);
	DefaultSensitivity = UKismetMathLibrary::GridSnap_Float(DefaultSensitivity, StepSize);

	CurrentSensitivity = DefaultSensitivity;
}

void USensitivitySetting::Init()
{
	Super::Init();

	DefaultSensitivity = CurrentSensitivity;

	Slider = Cast<USlider>(WidgetTree->FindWidget(FName("SensitivitySlider")));
	Value = Cast<UTextBlock>(WidgetTree->FindWidget(FName("Value")));
}

void USensitivitySetting::Apply()
{
}

void USensitivitySetting::Reset()
{
	CurrentSensitivity = DefaultSensitivity;

	ChangeSensitivity(GetSliderValueAtSensitivity(CurrentSensitivity));
	Slider->SetValue(GetSliderValueAtDefaultSensitivity());
}

bool USensitivitySetting::IsDefault()
{
	return CurrentSensitivity == DefaultSensitivity;
}

void USensitivitySetting::ChangeSensitivity(float SliderValue)
{
	SliderValue = UKismetMathLibrary::GridSnap_Float(SliderValue, StepSize);

	CurrentSensitivity = int32(FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(MinSensitivity, MaxSensitivity), SliderValue));

	Slider->SetValue(SliderValue);
	Value->SetText(FText::AsNumber(CurrentSensitivity));

	Apply();
}

float USensitivitySetting::GetSliderValueAtDefaultSensitivity()
{
	return FMath::GetMappedRangeValueClamped(FVector2D(MinSensitivity, MaxSensitivity), FVector2D(0.0f, 1.0f), DefaultSensitivity);
}

float USensitivitySetting::GetSliderValueAtSensitivity(const float Value)
{
	return FMath::GetMappedRangeValueClamped(FVector2D(MinSensitivity, MaxSensitivity), FVector2D(0.0f, 1.0f), Value);
}




