// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/ControlSetting.h"
#include "InvertSetting.generated.h"

/**
 * Manages the invertion setting of a controller's axis (e.g mouse, gamepad, etc.)
 */
UCLASS()
class MENUSYSTEM_API UInvertSetting final : public UControlSetting
{
	GENERATED_BODY()
	
public:
	void Init() override;
	void Apply() override;
	void Reset() override;

	bool IsDefault() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Mouse Invert Setting")
		void ChangeMouseInvert(const FString& SelectedItem);

	UFUNCTION(BlueprintCallable, Category = "Mouse Invert Setting")
		void PopulateList(class UComboBoxString* DropDownList);

	UFUNCTION(BlueprintCallable, Category = "Mouse Invert Setting")
		void SetSelectedOption(class UComboBoxString* DropDownList);
	
	UPROPERTY(EditInstanceOnly, Category = "Mouse Invert Setting")
		EMouseAxis Axis;

	UPROPERTY(EditInstanceOnly, Category = "Mouse Invert Setting")
		FString SelectedOption;

	UPROPERTY(EditInstanceOnly, Category = "Mouse Invert Setting")
		TArray<FString> Options;

	FString DefaultOption;
};
