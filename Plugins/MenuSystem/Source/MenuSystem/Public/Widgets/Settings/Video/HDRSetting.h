// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/VideoSetting.h"
#include "HDRSetting.generated.h"

/**
 * Manages the HDR enabling and disabling setting
 */
UCLASS()
class MENUSYSTEM_API UHDRSetting final : public UVideoSetting
{
	GENERATED_BODY()
	
public:
	void Init() override;
	void Apply() override;

	bool HasChanged() override;
	void RevertChange() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "HDR Setting")
		void ChangeHDRSetting(const FString& SelectedItem);

	UFUNCTION(BlueprintCallable, Category = "HDR Setting")
		void PopulateList(class UComboBoxString* DropDownList);

	UFUNCTION(BlueprintCallable, Category = "HDR Setting")
		void SetSelectedOption(class UComboBoxString* DropDownList);

	UPROPERTY(EditInstanceOnly, Category = "HDR Setting")
		FString SelectedOption;

	UPROPERTY(EditInstanceOnly, Category = "HDR Setting")
		TArray<FString> Options;

private:
	bool bHDREnabled;
};
