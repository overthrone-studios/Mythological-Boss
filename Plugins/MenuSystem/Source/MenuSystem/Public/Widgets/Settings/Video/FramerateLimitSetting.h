// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/VideoSetting.h"
#include "FramerateLimitSetting.generated.h"

/**
 * Manages the framerate of the game
 */
UCLASS()
class MENUSYSTEM_API UFramerateLimitSetting final : public UVideoSetting
{
	GENERATED_BODY()
	
public:
	void Init() override;
	void Apply() override;
	void Reset() override;

	void RevertChange() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Framerate Limit Setting")
		void ChangeFrameRate(const FString& SelectedItem);

	UFUNCTION(BlueprintCallable, Category = "Framerate Limit Setting")
		void PopulateList(class UComboBoxString* DropDownList);

	UFUNCTION(BlueprintCallable, Category = "Framerate Limit Setting")
		void SetSelectedOption(class UComboBoxString* DropDownList);

private:
	float NewFrameRateLimit;
};
