// Copyright Overthrone Studios 2019

#pragma once

#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * The main HUD the player will use when playing the game
 */
UCLASS()
class MYTHOLOGICALBOSS_API APlayerHUD final : public AHUD
{
	GENERATED_BODY()

public:
	APlayerHUD();

	void Init();

	void HighlightState(const FString& StateName);
	void UpdateStateUptime(const FString& StateName, float Uptime);
	void UnhighlightState(const FString& StateName);

protected:
	void BeginPlay() override;

	void CreateWidgets();
	void AddWidgetsToScreen();
	void InitWidgets();

	class UUserWidget* MasterHUD{};
	class UUserWidget* IdleStateWidget{};
	class UUserWidget* WalkStateWidget{};
	class UUserWidget* JumpStateWidget{};
	class UUserWidget* FallStateWidget{};
	class UUserWidget* AttackStateWidget{};
	class UUserWidget* BlockStateWidget{};
	TSubclassOf<class UUserWidget> WidgetClass;
};
