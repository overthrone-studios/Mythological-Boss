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

void UFSMVisualizerHUD::UpdateStateUptime(const FString& StateName, const float Uptime)
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

void UFSMVisualizerHUD::UnhighlightState(const FString& StateName)
{
	for (auto StateWidget : StateDebugWidgets)
	{
		if (StateWidget->GetStateName() == StateName)
		{
			StateWidget->ChangeBackgroundColor(White);
			StateWidget->UpdateUptimeText(FText::AsNumber(0.0f));
			return;
		}
	}
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
