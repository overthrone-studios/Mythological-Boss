// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/MenuSetting.h"
#include "ControlSetting.generated.h"

UENUM()
enum class EMouseAxis : uint8
{
	X UMETA(DisplayName="X"),
	Y UMETA(DisplayName="Y"),
};

/**
 * Base class for a setting in the controls menu
 */
UCLASS()
class MENUSYSTEM_API UControlSetting : public UMenuSetting
{
	GENERATED_BODY()
	
protected:
	class UComboBoxString* DropDownList;
	class USlider* Slider;
};
