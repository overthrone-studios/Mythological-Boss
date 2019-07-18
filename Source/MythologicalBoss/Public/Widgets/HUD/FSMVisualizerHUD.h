// Copyright Overthrone Studios 2019

#pragma once

#include "Widgets/HUD/HUDBase.h"
#include "FSMVisualizerHUD.generated.h"

/**
 * A widget that visualizes the player's state machines
 */
UCLASS()
class MYTHOLOGICALBOSS_API UFSMVisualizerHUD final : public UHUDBase
{
	GENERATED_BODY()
	
public:
	void Init() override;

	void HighlightState(const FString& StateName);
	void UpdateStateUptime(const FString& StateName, float Uptime);
	void UnhighlightState(const FString& StateName);

protected:
	TArray<class UStateDebugWidget*> GetAllStateDebugWidgets();
	TArray<class UStateDebugWidget*> StateDebugWidgets;
};
