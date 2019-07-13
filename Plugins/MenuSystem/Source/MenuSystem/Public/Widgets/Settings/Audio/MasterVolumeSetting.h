// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/AudioSetting.h"
#include "MasterVolumeSetting.generated.h"

/**
 * Manages the master volume of the game
 */
UCLASS()
class MENUSYSTEM_API UMasterVolumeSetting final : public UAudioSetting
{
	GENERATED_BODY()
	
public:
	void Init() override;

protected:
	UPROPERTY(config, BlueprintReadOnly, Category = "Audio")
		float MasterVolume = 100.0f;

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void ChangeMasterVolume(float SliderValue);
};
