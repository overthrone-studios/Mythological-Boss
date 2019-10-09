// Copyright Overthrone Studios 2019

#pragma once

#include "Widgets/HUD/HUDBase.h"
#include "FSMVisualizerHUD.generated.h"

/**
 * A widget that visualizes an actor's state machine
 */
UCLASS()
class MYTHOLOGICALBOSS_API UFSMVisualizerHUD final : public UHUDBase
{
	GENERATED_BODY()
	
public:
	void Init() override;

	void HighlightState(const FString& StateName);
	void UpdateStateUptime(const FString& StateName, const float& Uptime);
	void UpdatePreviousStateUptime(const FString& StateName, const float& Uptime);
	void UpdateStateFrames(const FString& StateName, const uint32& Frames);
	void UpdatePreviousStateFrames(const FString& StateName, const uint32& Frames);
	void UnhighlightState(const FString& StateName);

	void UpdatePreviousState(const FString& StateName, const float& Uptime, const uint32& Frames);

protected:
	TArray<class UStateDebugWidget*> GetAllStateDebugWidgets();
	TArray<class UStateDebugWidget*> StateDebugWidgets;

	class UStateDebugWidget* PreviousStateWidget;
};
