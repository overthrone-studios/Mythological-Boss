// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/GraphicsSetting.h"
#include "ViewDistanceSetting.generated.h"

/**
 * Manages the view distance quality of the game
 */
UCLASS()
class MENUSYSTEM_API UViewDistanceSetting final : public UGraphicsSetting
{
	GENERATED_BODY()
	
public:
	void Apply() override;
};
