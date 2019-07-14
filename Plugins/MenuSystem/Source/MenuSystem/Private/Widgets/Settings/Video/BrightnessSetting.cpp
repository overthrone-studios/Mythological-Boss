// Copyright Ali El Saleh 2019

#include "BrightnessSetting.h"
#include "WidgetTree.h"
#include "Slider.h"
#include "TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UBrightnessSetting::Init()
{
	Super::Init();

	Slider = Cast<USlider>(WidgetTree->FindWidget(FName("BrightnessSlider")));
	Value = Cast<UTextBlock>(WidgetTree->FindWidget(FName("Value")));

	Slider->SetValue(GetSliderValueAtBrightness(CurrentBrightness));

	FNumberFormattingOptions NumberFormatting;
	NumberFormatting.MaximumFractionalDigits = 1;
	Value->SetText(FText::AsNumber(CurrentBrightness, &NumberFormatting));

	const FString Command = FString("gamma ") + FString::SanitizeFloat(CurrentBrightness);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand(Command);

	bInitialized = true;
}

void UBrightnessSetting::Apply()
{
	const FString Command = FString("gamma ") + FString::SanitizeFloat(CurrentBrightness);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand(Command);

	SaveConfig(CPF_Config, *GGameUserSettingsIni);
}

void UBrightnessSetting::Reset()
{
	CurrentBrightness = DefaultBrightness;

	ChangeBrightnessSetting(GetSliderValueAtBrightness(CurrentBrightness));
	Slider->SetValue(GetSliderValueAtDefaultBrightness());
}

bool UBrightnessSetting::IsDefault()
{
	return CurrentBrightness == DefaultBrightness;
}

void UBrightnessSetting::NativePreConstruct()
{
	Super::NativePreConstruct();

	CurrentBrightness = DefaultBrightness;
}

void UBrightnessSetting::ChangeBrightnessSetting(const float SliderValue)
{
	if (bInitialized)
	{
		CurrentBrightness = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(MinBrightness, MaxBrightness), SliderValue);

		FNumberFormattingOptions NumberFormatting;
		NumberFormatting.MaximumFractionalDigits = 1;

		Value->SetText(FText::AsNumber(CurrentBrightness, &NumberFormatting));

		if (bApplyOnChange)
			Apply();
	}
}

float UBrightnessSetting::GetSliderValueAtDefaultBrightness()
{
	return FMath::GetMappedRangeValueClamped(FVector2D(MinBrightness, MaxBrightness), FVector2D(0.0f, 1.0f), DefaultBrightness);
}

float UBrightnessSetting::GetSliderValueAtBrightness(const float Value)
{
	return FMath::GetMappedRangeValueClamped(FVector2D(MinBrightness, MaxBrightness), FVector2D(0.0f, 1.0f), Value);
}
