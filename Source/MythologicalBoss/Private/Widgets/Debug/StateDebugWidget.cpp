// Copyright Overthrone Studios 2019

#include "Widgets/Debug/StateDebugWidget.h"
#include "WidgetTree.h"
#include "TextBlock.h"
#include "Image.h"
#include "Log.h"

void UStateDebugWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	LabelWidget = Cast<UTextBlock>(WidgetTree->FindWidget("Label"));
	BGImage = Cast<UImage>(WidgetTree->FindWidget("BGImage"));

	if (LabelWidget)
		LabelWidget->SetText(StateName);
}

void UStateDebugWidget::ChangeBackgroundColor(const FLinearColor& Color)
{
	if (BGImage)
		BGImage->SetColorAndOpacity(Color);
	else
		ULog::LogDebugMessage(ERROR, FString("Could not change background color."), true);
}

void UStateDebugWidget::UpdateUptimeText(const FText& NewUptimeText)
{
	UptimeText = NewUptimeText;
}
