// Copyright Ali El Saleh 2019

#include "FOVSetting.h"
#include "WidgetTree.h"
#include "Slider.h"
#include "TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UFOVSetting::NativePreConstruct()
{
	Super::NativePreConstruct();

	//if (DefaultFOV < MinFOV)
	//	DefaultFOV = MinFOV;
	//
	//if (DefaultFOV > MaxFOV)
	//	DefaultFOV = MaxFOV;
	//
	//CurrentFOV = DefaultFOV;
}

void UFOVSetting::ChangeFOVSetting(const float SliderValue)
{
	if (bInitialized)
	{
		CurrentFOV = int32(FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(float(MinFOV), float(MaxFOV)), SliderValue));

		Value->SetText(FText::AsNumber(CurrentFOV));

		if (bApplyOnChange)
			Apply();
	}
}

float UFOVSetting::GetSliderValueAtDefaultFOV()
{
	return FMath::GetMappedRangeValueClamped(FVector2D(float(MinFOV), float(MaxFOV)), FVector2D(0.0f, 1.0f), DefaultFOV);
}

float UFOVSetting::GetSliderValueAtFOV(const int32 Value)
{
	return FMath::GetMappedRangeValueClamped(FVector2D(float(MinFOV), float(MaxFOV)), FVector2D(0.0f, 1.0f), Value);
}

void UFOVSetting::Init()
{
	Super::Init();

	Slider = Cast<USlider>(WidgetTree->FindWidget(FName("FOVSlider")));
	Value = Cast<UTextBlock>(WidgetTree->FindWidget(FName("Value")));

	Slider->SetValue(GetSliderValueAtFOV(CurrentFOV));
	Value->SetText(FText::AsNumber(CurrentFOV));
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->SetFOV(CurrentFOV);

	bInitialized = true;
}

void UFOVSetting::Apply()
{
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->SetFOV(CurrentFOV);

	SaveConfig(CPF_Config, *GGameUserSettingsIni);
}

void UFOVSetting::Reset()
{
	CurrentFOV = DefaultFOV;

	ChangeFOVSetting(GetSliderValueAtFOV(CurrentFOV));
	Slider->SetValue(GetSliderValueAtDefaultFOV());
}

bool UFOVSetting::IsDefault()
{
	return CurrentFOV == DefaultFOV;
}
