// Copyright Ali El Saleh 2019

#pragma once

#include "Settings/VideoSetting.h"
#include "GraphicsSetting.generated.h"

/**
 * Base class of a graphics setting
 */
UCLASS()
class MENUSYSTEM_API UGraphicsSetting : public UVideoSetting
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Graphics Setting")
		virtual void ChangeGraphicsSetting(const FString& SelectedItem);
	
	UFUNCTION(BlueprintCallable, Category = "Graphics Setting")
		virtual void PopulateList(class UComboBoxString* DropDownList);
	
	UFUNCTION(BlueprintCallable, Category = "Graphics Setting")
		virtual void SetSelectedOption(class UComboBoxString* DropDownList);

	UPROPERTY(EditInstanceOnly, Category = "Graphics Setting")
		FString SelectedOption;

	UPROPERTY(EditInstanceOnly, Category = "Graphics Setting")
		TArray<FString> Options;

	int32 QualityIndex;
};
