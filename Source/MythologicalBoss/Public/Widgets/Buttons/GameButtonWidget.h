// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "TextBlock.h"
#include "TextWidgetTypes.h"
#include "GameButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYTHOLOGICALBOSS_API UGameButtonWidget final : public UButton
{
	GENERATED_BODY()

public:
	UGameButtonWidget();

protected:
	TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Button Text")
		FText ButtonName = FText::FromString("None");

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Button Text")
		FText ButtonTooltip = FText::FromString("None");

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Button Text")
		FMargin Padding = FMargin(4.0f, 2.0f);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Button Text")
		TEnumAsByte<EHorizontalAlignment> HorizontalAlignment = HAlign_Left;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Button Text")
		TEnumAsByte<EVerticalAlignment> VerticalAlignment;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Button Text")
		FSlateColor HoveredColorAndOpacity = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Button Text")
		FSlateColor UnhoveredColorAndOpacity = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.8f));

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Button Text")
		FSlateFontInfo TextInfo;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Button Text")
		TEnumAsByte<ETextJustify::Type> Justification = ETextJustify::Center;

	UFUNCTION(BlueprintCallable, Category = "Menu Button")
		void OnButtonHovered();

	UFUNCTION(BlueprintCallable, Category = "Menu Button")
		void OnButtonUnhovered();

private:
	class UTextBlock* TextBlock;
};
