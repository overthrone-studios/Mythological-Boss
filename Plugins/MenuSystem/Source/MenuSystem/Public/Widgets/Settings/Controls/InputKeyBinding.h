// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/ControlSetting.h"
#include "InputKeyBinding.generated.h"

/**
 * An input key binding setting widget
 */
UCLASS()
class MENUSYSTEM_API UInputKeyBinding final : public UControlSetting
{
	GENERATED_BODY()
	
public:
	void Init() override;
	void Reset() override;

	bool IsDefault() override;

	bool IsNegativeAxis() const {return Scale < 0.0f;}
	bool IsAxis() const {return bIsAxis;}
	FName GetInputName() const {return InputName;}
	float GetScale() const {return Scale;}

	FInputChord GetCurrentPrimaryInput() const {return CurrentPrimaryInput;}
	FInputChord GetCurrentGamepadInput() const {return CurrentGamepadInput;}

	FInputChord GetDefaultPrimaryInput() const {return CurrentPrimaryInput;}
	FInputChord GetDefaultGamepadInput() const {return DefaultGamepadInput;}

	FInputChord GetSelectedPrimaryKey() const;

	void SetCurrentPrimaryInput(const FInputChord& NewInput) {CurrentPrimaryInput = NewInput;}
	void SetCurrentGamepadInput(const FInputChord& NewInput) {CurrentGamepadInput = NewInput;}

	static bool IsInputAGamepadKey(const FInputChord& NewInput);

	FInputChord DuplicatePrimaryInput;
	FInputChord DuplicateGamepadInput;

	FInputChord PreviousPrimaryInput;
	FInputChord PreviousGamepadInput;

	void SetSelectedPrimaryInput();
	void SetSelectedGamepadInput();

protected:
	UFUNCTION(BlueprintCallable, Category = "InputKeyBinding Setting")
		void SetDefaultInput(class UInputKeySelector* Primary, class UInputKeySelector* Gamepad);

	UFUNCTION(BlueprintCallable, Category = "InputKeyBinding Setting")
		void SetCurrentInput(class UInputKeySelector* Primary, class UInputKeySelector* Gamepad);

	UFUNCTION(BlueprintCallable, Category = "InputKeyBinding Setting")
		void SetSelectedPrimaryInput(class UInputKeySelector* Primary);
	
	UFUNCTION(BlueprintCallable, Category = "InputKeyBinding Setting")
		void SetSelectedGamepadInput(class UInputKeySelector* Gamepad);

	UFUNCTION(BlueprintCallable, Category = "InputKeyBinding Setting")
		void RebindPrimaryInput(const FInputChord& NewInput);

	UFUNCTION(BlueprintCallable, Category = "InputKeyBinding Setting")
		void RebindGamepadInput(const FInputChord& NewInput);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Input Key Binding Setting")
		FName InputName;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Input Key Binding Setting")
		bool bIsAxis;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Input Key Binding Setting", meta = (EditCondition="bIsAxis", ClampMin=-1.0f, ClampMax=1.0f))
		float Scale = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Input Key Binding Setting")
		FInputChord DefaultPrimaryInput;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Input Key Binding Setting")
		FInputChord DefaultGamepadInput;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Input Key Binding Setting")
		FLinearColor DuplicateErrorColor = FLinearColor(FColor::Red);

private:
	class UInputKeySelector* PrimaryKeySelector;
	class UInputKeySelector* GamepadKeySelector;

	FInputChord CurrentPrimaryInput;
	FInputChord CurrentGamepadInput;

	class UControlsMenu* ControlsMenu;
};
