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

protected:
	UFUNCTION(BlueprintCallable, Category = "InputKeyBinding Setting")
		void SetDefaultInputChord(class UInputKeySelector* Primary, class UInputKeySelector* Gamepad);

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
