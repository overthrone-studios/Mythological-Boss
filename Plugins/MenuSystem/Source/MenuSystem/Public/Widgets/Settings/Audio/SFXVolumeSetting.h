// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/AudioSetting.h"
#include "SFXVolumeSetting.generated.h"

/**
 * Manages the sound effects volume in the game
 */
UCLASS()
class MENUSYSTEM_API USFXVolumeSetting : public UAudioSetting
{
	GENERATED_BODY()
	
public:
	void Init() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Audio")
		void ChangeSFXVolume(float SliderValue);
	
	UPROPERTY(config, BlueprintReadOnly, Category = "Audio")
		float SFXVolume = 100.0f;
};
