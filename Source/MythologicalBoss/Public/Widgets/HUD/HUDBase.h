// Copyright Overthrone Studios 2019

#pragma once

#include "Blueprint/UserWidget.h"
#include "HUDBase.generated.h"

/**
 * Base class for a HUD displayed on screen to the user
 */
UCLASS()
class MYTHOLOGICALBOSS_API UHUDBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init();

protected:

	class AOverthroneHUD* OverthroneHUD;
};
