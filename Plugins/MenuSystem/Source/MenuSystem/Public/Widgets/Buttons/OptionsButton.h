// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Buttons/ButtonBase.h"
#include "OptionsButton.generated.h"

/**
 * A button that lives within an options menu
 */
UCLASS()
class MENUSYSTEM_API UOptionsButton final : public UButtonBase
{
	GENERATED_BODY()
	
public:
	void Init() override;

private:
	class UOptionsMenu* OptionsMenu;
};
