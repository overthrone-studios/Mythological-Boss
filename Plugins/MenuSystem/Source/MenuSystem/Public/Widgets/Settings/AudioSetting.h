// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/MenuSetting.h"
#include "AudioSetting.generated.h"

/**
 * Base class for any type of video setting
 */
UCLASS(config=Game)
class MENUSYSTEM_API UAudioSetting : public UMenuSetting
{
	GENERATED_BODY()

public:
	UAudioSetting();

	void Init() override;

protected:
	UFUNCTION(BlueprintPure, Category = "Audio Setting")
		float GetSliderValueAtDefaultVolume();

	UFUNCTION(BlueprintPure, Category = "Audio Setting") 
		static float GetSliderValueAtVolume(float Value);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Audio Setting", meta = (ClampMin=0, ClampMax=100))
		int32 DefaultVolume = 100;

	class USlider* VolumeSlider;
	class UTextBlock* ValueWidget;

	class USoundClass* MasterSoundClass;
	class USoundClass* MusicSoundClass;
	class USoundClass* SFXSoundClass;
	class USoundClass* UISoundClass;
	class USoundClass* EnvironmentSoundClass;

	class USoundMix* MasterMix;
};
