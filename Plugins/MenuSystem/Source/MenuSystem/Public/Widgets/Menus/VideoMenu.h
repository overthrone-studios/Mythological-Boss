// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Menus/MenuBase.h"
#include "VideoMenu.generated.h"

/**
 * A video menu contains all settings that changes the visuals
 */
UCLASS()
class MENUSYSTEM_API UVideoMenu final : public UMenuBase
{
	GENERATED_BODY()
	
public:
	void Init() override;
	void Back() override;
	void Apply() override;

	void ShowResetWarning();
	void HideResetWarning();

	UFUNCTION(BlueprintCallable, Category = "Video Menu")
		void DiscardChanges();

protected:
	void GoBack() override;

	void HideWidgets();

	bool AnyUnsavedChanges();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Video Menu")
		bool bApplyOnLaunch;

	class UCanvasPanel* ResetWarningBox;
	class UCanvasPanel* ConfirmationBox;

	TArray<class UMenuSetting*> ChangedSettings;
};
