// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Buttons/ButtonBase.h"
#include "VideoButton.generated.h"

/**
 * A button that lives within a video menu
 */
UCLASS()
class MENUSYSTEM_API UVideoButton final : public UButtonBase
{
	GENERATED_BODY()
	
public:
	void Init() override;

protected:
	// For button functionality
	void OnButtonReleased() override;

private:
	class UVideoMenu* VideoMenu;
};
