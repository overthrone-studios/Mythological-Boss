// Copyright Overthrone Studios 2019

#pragma once

#include "Blueprint/UserWidget.h"
#include "StateDebugWidget.generated.h"

/**
 * A widget representing a state from a state machine
 */
UCLASS()
class MYTHOLOGICALBOSS_API UStateDebugWidget final : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE FString GetStateName() const { return StateName.ToString(); }

	void ChangeBackgroundColor(const FLinearColor& Color);
	void UpdateUptimeText(const FText& NewUptimeText);

protected:
	void NativePreConstruct() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "State")
		FText StateName = FText::FromString("None");
			
	UPROPERTY(BlueprintReadOnly, Category = "State")
		FText UptimeText = FText::AsNumber(0.0f);

	class UTextBlock* LabelWidget;
	class UImage* BGImage;
};
