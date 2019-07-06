// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Buttons/ButtonBase.h"
#include "AudioButton.generated.h"

/**
 * A button that lives within an audio menu
 */
UCLASS()
class MENUSYSTEM_API UAudioButton : public UButtonBase
{
	GENERATED_BODY()
	
public:
	void Init() override;

protected:
	// For button functionality
	void OnButtonReleased() override;

private:
	class UAudioMenu* AudioMenu;
	
};
