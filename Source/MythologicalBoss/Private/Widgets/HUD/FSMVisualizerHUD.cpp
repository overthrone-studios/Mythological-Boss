// Copyright Overthrone Studios 2019

#include "Widgets/HUD/FSMVisualizerHUD.h"
#include "Widgets/Debug/StateDebugWidget.h"
#include "WidgetTree.h"

static FLinearColor White = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
static FLinearColor Green = FLinearColor(0.187166f, 1.0f, 0.198517f, 1.0f);

void UFSMVisualizerHUD::Init()
{
	Super::Init();

	StateDebugWidgets = GetAllStateDebugWidgets();

	PreviousStateWidget = Cast<UStateDebugWidget>(WidgetTree->FindWidget("PreviousState"));
}

void UFSMVisualizerHUD::HighlightState(const FString& StateName)
{
	for (auto StateWidget : StateDebugWidgets)
	{
		if (StateWidget->GetStateName() == StateName)
		{
			StateWidget->ChangeBackgroundColor(Green);
			return;
		}
	}
}

void UFSMVisualizerHUD::UpdateStateUptime(const FString& StateName, const float& Uptime)
{
	for (auto StateWidget : StateDebugWidgets)
	{
		if (StateWidget->GetStateName() == StateName)
		{
			FNumberFormattingOptions NumberFormattingOptions;
			NumberFormattingOptions.MaximumFractionalDigits = 2;
			StateWidget->UpdateUptimeText(FText::AsNumber(Uptime, &NumberFormattingOptions));

			return;
		}
	}
}

void UFSMVisualizerHUD::UpdatePreviousStateUptime(const FString& StateName, const float& Uptime)
{
	for (auto StateWidget : StateDebugWidgets)
	{
		if (StateWidget->GetStateName() == StateName)
		{
			FNumberFormattingOptions NumberFormattingOptions;
			NumberFormattingOptions.MaximumFractionalDigits = 2;
			StateWidget->UpdatePreviousUptimeText(FText::AsNumber(Uptime, &NumberFormattingOptions));

			return;
		}
	}
}

void UFSMVisualizerHUD::UpdateStateFrames(const FString& StateName, const uint32& Frames)
{
	for (auto StateWidget : StateDebugWidgets)
	{
		if (StateWidget->GetStateName() == StateName)
		{
			FNumberFormattingOptions NumberFormattingOptions;
			NumberFormattingOptions.MaximumFractionalDigits = 0;
			StateWidget->UpdateFramesText(FText::AsNumber(Frames, &NumberFormattingOptions));

			return;
		}
	}
}

void UFSMVisualizerHUD::UpdatePreviousStateFrames(const FString& StateName, const uint32& Frames)
{
	for (auto StateWidget : StateDebugWidgets)
	{
		if (StateWidget->GetStateName() == StateName)
		{
			FNumberFormattingOptions NumberFormattingOptions;
			NumberFormattingOptions.MaximumFractionalDigits = 0;
			StateWidget->UpdatePreviousFramesText(FText::AsNumber(Frames, &NumberFormattingOptions));

			return;
		}
	}
}

void UFSMVisualizerHUD::UnhighlightState(const FString& StateName)
{
	for (auto StateWidget : StateDebugWidgets)
	{
		if (StateWidget->GetStateName() == StateName)
		{
			StateWidget->ChangeBackgroundColor(White);
			StateWidget->UpdateUptimeText(FText::AsNumber(0.0f));
			StateWidget->UpdateFramesText(FText::AsNumber(0));
			return;
		}
	}
}

void UFSMVisualizerHUD::UpdatePreviousState(const FString& StateName, const float& Uptime, const uint32& Frames)
{
	if (PreviousStateWidget)
		PreviousStateWidget->UpdateWidgetInfo(FText::FromString(StateName), FText::AsNumber(Uptime), FText::AsNumber(Frames));
}

TArray<UStateDebugWidget*> UFSMVisualizerHUD::GetAllStateDebugWidgets()
{
	TArray<UStateDebugWidget*> FoundWidgets;
	TArray<UWidget*> WidgetsInTree;

	WidgetTree->GetAllWidgets(WidgetsInTree);

	for (auto Widget : WidgetsInTree)
	{
		if (Widget->IsA(UStateDebugWidget::StaticClass()))
		{
			FoundWidgets.Add(Cast<UStateDebugWidget>(Widget));
		}
	}

	return FoundWidgets;
}
