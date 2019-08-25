// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Menus/MenuBase.h"
#include "OptionsMenu.generated.h"

/**
 * Base class of an options menu widget blueprint
 */
UCLASS()
class MENUSYSTEM_API UOptionsMenu final : public UMenuBase
{
	GENERATED_BODY()
	
public:
	void Back() override;

protected:
	void GoBack() override;
};
