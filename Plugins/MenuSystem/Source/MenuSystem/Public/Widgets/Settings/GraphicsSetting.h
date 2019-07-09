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

public:
	void Init() override;
	void Apply() override;
	void Reset() override;

	void RevertChange() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Graphics Setting")
		virtual void ChangeGraphicsSetting(const FString& SelectedItem);
	
	UFUNCTION(BlueprintCallable, Category = "Graphics Setting")
		virtual void PopulateList(class UComboBoxString* DropDownList);
	
	UFUNCTION(BlueprintCallable, Category = "Graphics Setting")
		virtual void SetSelectedOption(class UComboBoxString* DropDownList);

	int32 QualityIndex;
};
