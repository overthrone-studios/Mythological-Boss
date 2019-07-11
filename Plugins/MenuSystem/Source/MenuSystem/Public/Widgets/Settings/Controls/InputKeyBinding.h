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

	bool IsAxis() const {return bIsAxis;}
	FName GetInputName() const {return InputName;}
	float GetScale() const {return Scale;}

	FInputChord GetCurrentPrimaryInput() const {return CurrentPrimaryInput;}
	FInputChord GetCurrentGamepadInput() const {return CurrentGamepadInput;}

	FInputChord GetDefaultPrimaryInput() const {return CurrentPrimaryInput;}
	FInputChord GetDefaultGamepadInput() const {return DefaultGamepadInput;}

	void SetCurrentPrimaryInput(const FInputChord& NewInput) {CurrentPrimaryInput = NewInput;}
	void SetCurrentGamepadInput(const FInputChord& NewInput) {CurrentGamepadInput = NewInput;}

protected:
	UFUNCTION(BlueprintCallable, Category = "InputKeyBinding Setting")
		void SetDefaultInput(class UInputKeySelector* Primary, class UInputKeySelector* Gamepad);

	UFUNCTION(BlueprintCallable, Category = "InputKeyBinding Setting")
		void SetSelectedPrimaryInput(class UInputKeySelector* Primary);
	
	UFUNCTION(BlueprintCallable, Category = "InputKeyBinding Setting")
		void SetSelectedGamepadInput(class UInputKeySelector* Gamepad);

	UFUNCTION(BlueprintCallable, Category = "InputKeyBinding Setting")
		void UpdatePrimaryInput(const FInputChord& NewInput);

	UFUNCTION(BlueprintCallable, Category = "InputKeyBinding Setting")
		void UpdateGamepadInput(const FInputChord& NewInput);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Input Key Binding Setting")
		FName InputName;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Input Key Binding Setting")
		bool bIsAxis;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Input Key Binding Setting", meta = (EditCondition="bIsAxis", ClampMin=-1.0f, ClampMax=1.0f))
		float Scale = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Input Key Binding Setting")
		FInputChord CurrentPrimaryInput;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Input Key Binding Setting")
		FInputChord CurrentGamepadInput;
	
private:
	static bool IsInputAGamepadKey(const FInputChord& NewInput);

	class UInputKeySelector* PrimaryKeySelector;
	class UInputKeySelector* GamepadKeySelector;

	FInputChord DefaultPrimaryInput;
	FInputChord DefaultGamepadInput;

	class UControlsMenu* ControlsMenu;
};
