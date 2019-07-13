// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/VideoSetting.h"
#include "BrightnessSetting.generated.h"

/**
 * Manages the overal brightness setting of the game
 */
UCLASS(config=Game)
class MENUSYSTEM_API UBrightnessSetting final : public UVideoSetting
{
	GENERATED_BODY()
	
public:
	void Init() override;
	void Apply() override;
	void Reset() override;

	bool IsDefault() override;

protected:
	void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
		void ChangeBrightnessSetting(float SliderValue);

	UFUNCTION(BlueprintPure)
		float GetSliderValueAtDefaultBrightness();

	UFUNCTION(BlueprintPure)
		float GetSliderValueAtBrightness(float Value);

	UPROPERTY(config, VisibleAnywhere, BlueprintReadOnly, Category = "Brightness Setting")
		float CurrentBrightness = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Brightness Setting", meta = (ClampMin=0.5f, ClampMax=5.0f))
		float MinBrightness = 0.5f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Brightness Setting", meta = (ClampMin=0.5f, ClampMax=5.0f))
		float MaxBrightness = 5.0f;
	
	UPROPERTY(config, EditInstanceOnly, BlueprintReadWrite, Category = "Brightness Setting", meta = (ClampMin=0.5f, ClampMax=5.0f))
		float DefaultBrightness = 2.2f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Brightness Setting")
		bool bApplyOnChange;

	class UTextBlock* Value;
};
