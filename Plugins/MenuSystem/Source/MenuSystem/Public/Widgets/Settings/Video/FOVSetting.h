// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/VideoSetting.h"
#include "FOVSetting.generated.h"

/**
 * Manages the Field of View setting
 */
UCLASS(config=Game)
class MENUSYSTEM_API UFOVSetting final : public UVideoSetting
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
		void ChangeFOVSetting(float SliderValue);

	UFUNCTION(BlueprintPure)
		float GetSliderValueAtDefaultFOV();

	UFUNCTION(BlueprintPure)
		float GetSliderValueAtFOV(int32 Value);

	UPROPERTY(config, VisibleAnywhere, BlueprintReadOnly, Category = "FOV")
		int32 CurrentFOV = 90;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FOV")
		int32 MinFOV = 70;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FOV")
		int32 MaxFOV = 120;
	
	UPROPERTY(config, EditInstanceOnly, BlueprintReadWrite, Category = "FOV")
		int32 DefaultFOV = 90;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FOV")
		bool bApplyOnChange;

	class UTextBlock* Value;
};
