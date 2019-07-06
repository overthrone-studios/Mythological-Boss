// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Buttons/ButtonBase.h"
#include "ControlsButton.generated.h"

/**
 * A button that lives within the controls menu
 */
UCLASS()
class MENUSYSTEM_API UControlsButton final : public UButtonBase
{
	GENERATED_BODY()
	
public:
	void Init() override;

protected:
	// For button functionality
	void OnButtonReleased() override;

	UPROPERTY(BlueprintReadWrite, Category = "Menu Button")
		class UControlsMenu* ControlsMenu;
};
