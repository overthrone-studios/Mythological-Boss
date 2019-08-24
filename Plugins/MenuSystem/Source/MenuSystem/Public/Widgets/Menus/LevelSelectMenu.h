// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Menus/MenuBase.h"
#include "LevelSelectMenu.generated.h"

/**
 * 
 */
UCLASS()
class MENUSYSTEM_API ULevelSelectMenu final : public UMenuBase
{
	GENERATED_BODY()
	
protected:
	void Forward(EButtonType Menu) override;
	void GoForward() override;

	void Back() override;
	void GoBack() override;
};
