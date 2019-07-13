// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/AudioSetting.h"
#include "UIVolumeSetting.generated.h"

/**
 * Manages the menu UI volume of the game
 */
UCLASS()
class MENUSYSTEM_API UUIVolumeSetting final : public UAudioSetting
{
	GENERATED_BODY()
	
public:
	void Init() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Audio")
		void ChangeUIVolume(float SliderValue);

	UPROPERTY(config, BlueprintReadOnly, Category = "Audio")
		float UIVolume = 100.0f;
};
