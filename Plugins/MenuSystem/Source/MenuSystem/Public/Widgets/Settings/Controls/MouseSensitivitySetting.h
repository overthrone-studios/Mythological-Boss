// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/ControlSetting.h"
#include "MouseSensitivitySetting.generated.h"

UENUM()
enum class EMouseAxis : uint8
{
	X,
	Y
};

/**
 * Manages the mouse's sensitivity setting
 */
UCLASS()
class MENUSYSTEM_API UMouseSensitivitySetting final : public UControlSetting
{
	GENERATED_BODY()
	
public:
	void Init() override;
	void Apply() override;
	void Reset() override;

	bool IsDefault() override;

protected:
	void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Mouse Sensitivity")
		void ChangeSensitivity(float SliderValue);

	UFUNCTION(BlueprintPure, Category = "Mouse Sensitivity")
		float GetSliderValueAtDefaultSensitivity();

	UFUNCTION(BlueprintPure, Category = "Mouse Sensitivity")
		float GetSliderValueAtSensitivity(float Value);
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Mouse Sensitivity")
		EMouseAxis Axis;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mouse Sensitivity")
		float CurrentSensitivity = 5.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mouse Sensitivity", meta = (ClampMin=1.0f, ClampMax=10.0f))
		float MinSensitivity = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mouse Sensitivity", meta = (ClampMin=1.0f, ClampMax=10.0f))
		float MaxSensitivity = 10.0f;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Mouse Sensitivity", meta = (ClampMin=1.0f, ClampMax=10.0f))
		float DefaultSensitivity = 5.0f;

	float StepSize = 1/(MaxSensitivity-MinSensitivity);

	class USlider* Slider;
	class UTextBlock* Value;
};
