// Copyright Overthrone Studios 2019

#pragma once

#include "GameFramework/HUD.h"
#include "OverthroneHUD.generated.h"

/**
 * The main HUD the player will use when playing the game
 */
UCLASS()
class MYTHOLOGICALBOSS_API AOverthroneHUD final : public AHUD
{
	GENERATED_BODY()

public:
	AOverthroneHUD();

	void Init();

	FORCEINLINE class UMasterHUD* GetMasterHUD() const { return MasterHUD; }

protected:
	void CreateWidgets();
	void AddWidgetsToScreen();
	void InitWidgets();

	class UMasterHUD* MasterHUD{};
	TSubclassOf<class UHUDBase> HUDWidgetClass;

	class UUserWidget* IntroCutsceneWidget{};
	TSubclassOf<class UUserWidget> IntroCutsceneWidgetClass;

private:
	class UOverthroneGameInstance* GameInstance;
};
