// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Menus/MenuBase.h"
#include "ControlsMenu.generated.h"

/**
 * A controls menu is where you can change the controls of the game
 */
UCLASS()
class MENUSYSTEM_API UControlsMenu final : public UMenuBase
{
	GENERATED_BODY()
	
public:
	void ResetKeyBindings();

	void Init() override;
	void InitializeButtons() override;
	void InitializeControls();
	void Reset() override;

	void Back() override;
	void GoBack() override;

	void ShowResetWarning();
	void HideResetWarning();

	void RemoveAllKeyBindings();
	void RebindInputMapping(const class UInputKeyBinding* InputKeyBinding, const FInputChord& OldInput, const FInputChord& NewInput);

protected:
	TArray<class UInputKeyBinding*> GetAllControls();

	void HideWidgets();

	UUserWidget* ResetWarningBox;
};
