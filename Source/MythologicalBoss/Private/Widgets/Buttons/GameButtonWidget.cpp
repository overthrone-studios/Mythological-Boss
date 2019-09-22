// Copyright Overthrone Studios 2019

#include "Widgets/Buttons/GameButtonWidget.h"
#include "ButtonSlot.h"

UGameButtonWidget::UGameButtonWidget()
{
	HoveredColorAndOpacity = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	UnhoveredColorAndOpacity = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.8f));

	TextInfo.FontObject = StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("Font'/Engine/EngineFonts/Roboto.Roboto'"));
	TextInfo.TypefaceFontName = "Regular";
	TextInfo.Size = 34.0f;

	OnHovered.AddDynamic(this, &UGameButtonWidget::OnButtonHovered);
	OnUnhovered.AddDynamic(this, &UGameButtonWidget::OnButtonUnhovered);
}

TSharedRef<SWidget> UGameButtonWidget::RebuildWidget()
{
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (!TextBlock)
		TextBlock = NewObject<UTextBlock>();

	if (!HasChild(TextBlock))
		AddChild(TextBlock);

	auto ButtonSlot = Cast<UButtonSlot>(GetContentSlot());
	if (ButtonSlot)
	{
		ButtonSlot->SetPadding(Padding);
		ButtonSlot->SetHorizontalAlignment(HorizontalAlignment);
		ButtonSlot->SetVerticalAlignment(VerticalAlignment);
	}

	TextBlock->SetColorAndOpacity(UnhoveredColorAndOpacity);
	TextBlock->SetFont(TextInfo);
	TextBlock->SetJustification(Justification);

	TextBlock->SetText(ButtonName);

	return Widget;
}

void UGameButtonWidget::OnButtonHovered()
{
	TextBlock->SetColorAndOpacity(HoveredColorAndOpacity);
}

void UGameButtonWidget::OnButtonUnhovered()
{
	TextBlock->SetColorAndOpacity(UnhoveredColorAndOpacity);
}
