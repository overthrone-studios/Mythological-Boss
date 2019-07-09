// Copyright Ali El Saleh 2019

#pragma once

#include "Settings/MenuSetting.h"
#include "VideoSetting.generated.h"

/**
 * Base class for any type of video setting
 */
UCLASS()
class MENUSYSTEM_API UVideoSetting : public UMenuSetting
{
	GENERATED_BODY()

public:
	void Apply() override;
	bool HasChanged() override;
	bool IsDefault() override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Video Setting")
		FString SelectedOption;

	UPROPERTY(EditInstanceOnly, Category = "Video Setting")
		TArray<FString> Options;

	FString AppliedChange;
	FString DefaultOption;

	class UComboBoxString* DropDownList;
	class USlider* Slider;
};
