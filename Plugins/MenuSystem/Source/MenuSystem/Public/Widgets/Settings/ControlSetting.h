// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/MenuSetting.h"
#include "ControlSetting.generated.h"

/**
 * Base class for a setting in the controls menu
 */
UCLASS()
class MENUSYSTEM_API UControlSetting : public UMenuSetting
{
	GENERATED_BODY()
	
public:
	bool IsDefault() override;
	
};
