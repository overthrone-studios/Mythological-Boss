// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/GraphicsSetting.h"
#include "PostProcessSetting.generated.h"

/**
 * Manages the post processing effects of the game
 */
UCLASS()
class MENUSYSTEM_API UPostProcessSetting final : public UGraphicsSetting
{
	GENERATED_BODY()
	
public:
	void Apply() override;
};
