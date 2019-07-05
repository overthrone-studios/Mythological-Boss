// Copyright Ali El Saleh 2019

#pragma once

#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

#define MAIN_MENU 0
#define NEW_GAME_MENU 1
#define OPTIONS_MENU 2
#define VIDEO_MENU 3
#define CONTROLS_MENU 4
#define AUDIO_MENU 5
#define CREDITS_SCREEN 6

/**
 * A HUD that displays different menu screens to the user
 */
UCLASS()
class MENUSYSTEM_API AMenuHUD final : public AHUD
{
	GENERATED_BODY()
	
public:
	AMenuHUD();

	class UUserWidget* GetMenu(int32 Index);

	void ShowHUD() override;
	void HideHUD();

	void ShowMenu(int32 Index);
	void HideMenu(int32 Index);

	void SlideMainMenu();

protected:
	void BeginPlay() override;

	void CreateWidgets();
	void InitializeWidgets();
	void AddWidgetsToScreen();
	void LogWidgetFailures();

	bool AllWidgetsValid();

private:
	TSubclassOf<class UMenuBase> WidgetClass;
	class UMasterMenu* MasterMenu{};

};
