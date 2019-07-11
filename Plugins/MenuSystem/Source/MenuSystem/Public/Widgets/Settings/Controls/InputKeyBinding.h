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

protected:
	UFUNCTION(BlueprintCallable, Category = "InputKeyBinding Setting")
		void SetDefaultInput(class UInputKeySelector* Primary, class UInputKeySelector* Gamepad);

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
	class UInputKeySelector* PrimaryKeySelector;
	class UInputKeySelector* GamepadKeySelector;

	FInputChord DefaultPrimaryInput;
	FInputChord DefaultGamepadInput;
};
