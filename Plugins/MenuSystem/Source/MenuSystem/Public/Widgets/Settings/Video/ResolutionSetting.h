// Copyright Ali El Saleh 2019

#pragma once

#include "Settings/VideoSetting.h"
#include "ResolutionSetting.generated.h"

/**
 * Manages the change of screen resolution
 */
UCLASS()
class MENUSYSTEM_API UResolutionSetting final : public UVideoSetting
{
	GENERATED_BODY()
	
public:
	void Init() override;
	void Apply() override;
	void Reset() override;

	bool HasChanged() override;
	void RevertChange() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Video Setting")
		void ChangeResolution(const FString& SelectedItem);

	UFUNCTION(BlueprintCallable, Category = "Video Setting")
		void PopulateResolutionList(class UComboBoxString* DropDownList);
	
	UFUNCTION(BlueprintCallable, Category = "Video Setting")
		void SetSelectedOption(class UComboBoxString* DropDownList);

	UFUNCTION(BlueprintPure, Category = "Video Settings")
		TArray<FString> GetAllSupportedResolutions();

private:
	FIntPoint NewResolution;

	FScreenResolutionArray ScreenResolutions;
};
