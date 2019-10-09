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
	void UpdatePreviousUptimeText(const FText& NewUptimeText);
	void UpdateFramesText(const FText& NewFramesText);
	void UpdatePreviousFramesText(const FText& NewFramesText);

	void UpdateWidgetInfo(const FText& NewName, const FText& NewUptimeText, const FText& NewFramesText);

protected:
	void NativePreConstruct() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "State")
		FText StateName = FText::FromString("None");
			
	UPROPERTY(BlueprintReadOnly, Category = "State")
		FText UptimeText = FText::AsNumber(0.0f);

	UPROPERTY(BlueprintReadOnly, Category = "State")
		FText FramesText = FText::AsNumber(0);

	UPROPERTY(BlueprintReadOnly, Category = "State")
		FText PreviousUptimeText = FText::AsNumber(0.0f);

	UPROPERTY(BlueprintReadOnly, Category = "State")
		FText PreviousFramesText = FText::AsNumber(0);

	class UTextBlock* LabelWidget;
	class UImage* BGImage;
};
