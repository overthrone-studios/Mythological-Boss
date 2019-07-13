// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/AudioSetting.h"
#include "MusicVolumeSetting.generated.h"

/**
 * Manages the music volume of the game
 */
UCLASS()
class MENUSYSTEM_API UMusicVolumeSetting final : public UAudioSetting
{
	GENERATED_BODY()
	
public:
	void Init() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Audio")
		void ChangeMusicVolume(float SliderValue);
	
	UPROPERTY(config, BlueprintReadOnly, Category = "Audio")
		float MusicVolume = 100.0f;
};
