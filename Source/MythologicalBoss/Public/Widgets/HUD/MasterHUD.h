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

	UFUNCTION(BlueprintCallable, Category = "Master HUD")
		UHUDBase* GetHUD(const FString& HUDWidgetName) const;

	UFUNCTION(BlueprintCallable, Category = "Master HUD")
		TArray<UHUDBase*> GetAllHUDs() const;

	UFUNCTION(BlueprintCallable, Category = "Master HUD")
		UWidget* GetActiveHUDWidget() const;
	UFUNCTION(BlueprintCallable, Category = "Master HUD")
		int32 GetActiveHUDIndex() const;

	void StoreAllDebugOptions(const UPanelWidget* ParentWidget);

	UFUNCTION(BlueprintCallable, Category = "Master HUD")
		void SwitchToHUD(UHUDBase* HUD);
	UFUNCTION(BlueprintCallable, Category = "Master HUD")
		void SwitchToHUDIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Master HUD")
		void HighlightBox(int32 Index);
	UFUNCTION(BlueprintCallable, Category = "Master HUD")
		void UnhighlightBox(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Master HUD")
		void ShowDebugInfo();
	UFUNCTION(BlueprintCallable, Category = "Master HUD")
		void HideDebugInfo();

	UFUNCTION(BlueprintCallable, Category = "Master HUD")
		void ShowTitle();
	UFUNCTION(BlueprintCallable, Category = "Master HUD")
		void HideTitle();

	UFUNCTION(BlueprintCallable, Category = "Master HUD")
		void ShowBoxes();
	UFUNCTION(BlueprintCallable, Category = "Master HUD")
		void HideBoxes();

	uint8 bLogHUDInitialized : 1;

protected:
	TArray<UHUDBase*> GetAllChildHUDs() const;

	class UWidgetSwitcher* WidgetSwitcher;

	TArray<UHUDBase*> HUDs;

	TArray<UUserWidget*> Boxes;

	UPanelWidget* DebugOptions;
	class UTextBlock* Title;
};
