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
		ULog::DebugMessage(ERROR, FString("Could not change background color."), true);
}

void UStateDebugWidget::UpdateUptimeText(const FText& NewUptimeText)
{
	UptimeText = NewUptimeText;
}

void UStateDebugWidget::UpdatePreviousUptimeText(const FText& NewUptimeText)
{
	PreviousUptimeText = NewUptimeText;
}

void UStateDebugWidget::UpdateFramesText(const FText& NewFramesText)
{
	FramesText = NewFramesText;
}

void UStateDebugWidget::UpdatePreviousFramesText(const FText& NewFramesText)
{
	PreviousFramesText = NewFramesText;
}

void UStateDebugWidget::UpdateWidgetInfo(const FText& NewName, const FText& NewUptimeText, const FText& NewFramesText)
{
	if (LabelWidget)
		LabelWidget->SetText(NewName);

	UptimeText = NewUptimeText;
	FramesText = NewFramesText;
}
