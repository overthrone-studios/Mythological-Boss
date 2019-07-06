// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Menus/MenuBase.h"
#include "NewGameMenu.generated.h"

/**
 * A new game menu is a menu where you are prompted with starting a new game
 */
UCLASS()
class MENUSYSTEM_API UNewGameMenu final : public UMenuBase
{
	GENERATED_BODY()
		
protected:
	void NativeConstruct() override;
	
};
