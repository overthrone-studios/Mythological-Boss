// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/MenuSetting.h"
#include "AudioSetting.generated.h"

/**
 * Base class for any type of video setting
 */
UCLASS()
class MENUSYSTEM_API UAudioSetting : public UMenuSetting
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure, Category = "Audio Setting")
		float GetSliderValueAtDefaultVolume();

	UFUNCTION(BlueprintPure, Category = "Audio Setting")
		float GetSliderValueAtVolume(int32 Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Setting", meta = (ClampMin=0, ClampMax=100))
		int32 CurrentVolume = 100;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Audio Setting", meta = (ClampMin=0, ClampMax=100))
		int32 DefaultVolume = 100;
};
