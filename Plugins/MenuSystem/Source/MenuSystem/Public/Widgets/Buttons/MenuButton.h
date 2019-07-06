// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Buttons/ButtonBase.h"
#include "MenuButton.generated.h"

/**
 * A button that lives within a main menu
 */
UCLASS()
class MENUSYSTEM_API UMenuButton final : public UButtonBase
{
	GENERATED_BODY()
	
public:
	void Init() override;

protected:
	// For button functionality
	void OnButtonReleased() override;

private:
	class UMainMenu* MainMenu;	
};
