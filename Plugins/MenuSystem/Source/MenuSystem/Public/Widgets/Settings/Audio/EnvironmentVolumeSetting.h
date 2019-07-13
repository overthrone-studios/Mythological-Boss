// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/AudioSetting.h"
#include "EnvironmentVolumeSetting.generated.h"

/**
 * Manages the environment volume of the game
 */
UCLASS()
class MENUSYSTEM_API UEnvironmentVolumeSetting final : public UAudioSetting
{
	GENERATED_BODY()
	
public:
	void Init() override;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Audio")
		void ChangeEnvironmentVolume(float SliderValue);

	UPROPERTY(config, BlueprintReadOnly, Category = "Audio")
		float EnvironmentVolume = 100.0f;
};
