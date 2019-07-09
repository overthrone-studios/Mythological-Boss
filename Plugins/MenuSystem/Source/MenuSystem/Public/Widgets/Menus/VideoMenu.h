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

	void InitializeButtons() override;

	void ShowResetWarning();
	void HideResetWarning();

	void DiscardChanges();

	bool AreAllSettingsDefault();

protected:
	void GoBack() override;

	void HideWidgets();

	bool AnyUnsavedChanges();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Video Menu")
		bool bApplyOnLaunch;

	UUserWidget* ResetWarningBox;
	UUserWidget* ConfirmationBox;

	TArray<class UMenuSetting*> ChangedSettings;
};
