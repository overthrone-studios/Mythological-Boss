// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/AudioSetting.h"
#include "MasterVolumeSetting.generated.h"

/**
 * Controls the master volume of the game
 */
UCLASS()
class MENUSYSTEM_API UMasterVolumeSetting final : public UAudioSetting
{
	GENERATED_BODY()
	
public:
	void Apply() override;
};
