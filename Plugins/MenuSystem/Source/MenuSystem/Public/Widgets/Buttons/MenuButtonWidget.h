// Copyright Ali El Saleh 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "TextBlock.h"
#include "MenuButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class MENUSYSTEM_API UMenuButtonWidget final : public UButton
{
	GENERATED_BODY()

public:
	UMenuButtonWidget();

	UFUNCTION(BlueprintCallable, Category = "Menu Button")
		void OnButtonHovered();

	UFUNCTION(BlueprintCallable, Category = "Menu Button")
		void OnButtonUnhovered();

	FORCEINLINE FText GetButtonTooltip() const { return ButtonTooltip; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu Button")
		USoundBase* PressedSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu Button")
		USoundBase* HoveredSound;

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

private:
	class UTextBlock* TextBlock;
};
