// Copyright Ali El Saleh 2019

#pragma once

#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

/**
 * A HUD that displays different menu screens to the user
 */
UCLASS()
class MENUSYSTEM_API AMenuHUD final : public AHUD
{
	GENERATED_BODY()
	
public:
	AMenuHUD();

	class UUserWidget* GetMenu(TSubclassOf<class UMenuBase> MenuClass);

	void EnableGameInputMode();

	void ShowHUD() override;
	void HideHUD();

	void ShowMenu(TSubclassOf<class UMenuBase> MenuClass);
	void HideMenu(TSubclassOf<class UMenuBase> MenuClass);

	void ShowMenu(int32 Index);
	void HideMenu(int32 Index);

	void ShowMenu(const FString& MenuWidgetName);
	void HideMenu(const FString& MenuWidgetName);

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
