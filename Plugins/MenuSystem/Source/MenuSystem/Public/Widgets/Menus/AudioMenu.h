// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Menus/MenuBase.h"
#include "AudioMenu.generated.h"

/**
 * An audio menu is where all audio settings are managed
 */
UCLASS()
class MENUSYSTEM_API UAudioMenu final : public UMenuBase
{
	GENERATED_BODY()
		
public:
	void Init() override;

	void Back() override;

protected:
	void GoBack() override;
};
