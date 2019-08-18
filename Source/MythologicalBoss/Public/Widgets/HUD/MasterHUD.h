// Copyright Overthrone Studios 2019

#pragma once

#include "Widgets/HUD/HUDBase.h"
#include "MasterHUD.generated.h"

/**
 * The HUD that controls all HUDs in Overthrone and switches between them.
 */
UCLASS()
class MYTHOLOGICALBOSS_API UMasterHUD final : public UHUDBase
{
	GENERATED_BODY()
	
public:
	void Init() override;

	UHUDBase* GetHUD(TSubclassOf<UHUDBase> HUDClass) const;
	UHUDBase* GetHUD(const FString& HUDWidgetName) const;
	TArray<UHUDBase*> GetAllHUDs() const;

	UWidget* GetActiveHUDWidget() const;
	int32 GetActiveHUDIndex() const;

	void SwitchToHUD(UHUDBase* HUD);
	void SwitchToHUDIndex(int32 Index);

	void HighlightBox(int32 Index);
	void UnhighlightBox(int32 Index);

	void ShowTitle();
	void HideTitle();
	void ShowBoxes();
	void HideBoxes();

	uint8 bLogHUDInitialized : 1;

protected:
	TArray<UHUDBase*> GetAllChildHUDs() const;

	class UWidgetSwitcher* WidgetSwitcher;

	TArray<UHUDBase*> HUDs;

	TArray<UUserWidget*> Boxes;

	UPanelWidget* HUDOptions;
	class UTextBlock* Title;
};
