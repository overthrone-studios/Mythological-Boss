// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/VideoSetting.h"
#include "OverallQualitySetting.generated.h"

/**
 * Manages the overall graphics setting
 */
UCLASS()
class MENUSYSTEM_API UOverallQualitySetting final : public UVideoSetting
{
	GENERATED_BODY()
	
public:
	void Init() override;
	void Apply() override;

	void RevertChange() override;
	void Reset() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Overall Quality Setting")
		void ChangeAllQualitySettings(const FString& SelectedItem);
	
	UFUNCTION(BlueprintCallable, Category = "Overall Quality Setting")
		void PopulateList(class UComboBoxString* DropDownList);
	
	UFUNCTION(BlueprintCallable, Category = "Overall Quality Setting")
		void SetSelectedOption(class UComboBoxString* DropDownList);

	TArray<class UGraphicsSetting*> GetAllGraphicsSettings();

	int32 OverallQualityIndex;
};
