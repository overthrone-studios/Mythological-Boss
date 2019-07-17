// Copyright Overthrone Studios 2019

#include "Widgets/HUD/FSMVisualizerHUD.h"
#include "WidgetTree.h"
#include "Image.h"
#include "TextBlock.h"
#include "Log.h"

static FLinearColor White = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
static FLinearColor Green = FLinearColor(0.187166f, 1.0f, 0.198517f, 1.0f);

void UFSMVisualizerHUD::Init()
{
	Super::Init();

	IdleStateWidget = Cast<UUserWidget>(WidgetTree->FindWidget("IdleState"));
	WalkStateWidget = Cast<UUserWidget>(WidgetTree->FindWidget("WalkState"));
	JumpStateWidget = Cast<UUserWidget>(WidgetTree->FindWidget("JumpState"));
	FallStateWidget = Cast<UUserWidget>(WidgetTree->FindWidget("FallState"));
	BlockStateWidget = Cast<UUserWidget>(WidgetTree->FindWidget("BlockState"));
	AttackStateWidget = Cast<UUserWidget>(WidgetTree->FindWidget("AttackState"));
	RunStateWidget = Cast<UUserWidget>(WidgetTree->FindWidget("RunState"));
}

void UFSMVisualizerHUD::HighlightState(const FString& StateName)
{
	if (StateName == "Idle")
	{
		if (IdleStateWidget)
		{
			// Change background color to green
			const auto Background = Cast<UImage>(IdleStateWidget->WidgetTree->FindWidget("BGImage"));
			if (Background)
				Background->SetColorAndOpacity(Green);
		}
	}
	else if (StateName == "Walk")
	{
		if (WalkStateWidget)
		{
			// Change background color to green
			const auto Background = Cast<UImage>(WalkStateWidget->WidgetTree->FindWidget("BGImage"));
			if (Background)
				Background->SetColorAndOpacity(Green);
		}
	}
	else if (StateName == "Jump")
	{
		if (JumpStateWidget)
		{
			// Change background color to green
			const auto Background = Cast<UImage>(JumpStateWidget->WidgetTree->FindWidget("BGImage"));
			if (Background)
				Background->SetColorAndOpacity(Green);
		}
	}
	else if (StateName == "Falling")
	{
		if (FallStateWidget)
		{
			// Change background color to green
			const auto Background = Cast<UImage>(FallStateWidget->WidgetTree->FindWidget("BGImage"));
			if (Background)
				Background->SetColorAndOpacity(Green);
		}
	}
	else if (StateName == "Attack")
	{
		if (AttackStateWidget)
		{
			// Change background color to green
			const auto Background = Cast<UImage>(AttackStateWidget->WidgetTree->FindWidget("BGImage"));
			if (Background)
				Background->SetColorAndOpacity(Green);
		}
	}
	else if (StateName == "Block")
	{
		if (BlockStateWidget)
		{
			// Change background color to green
			const auto Background = Cast<UImage>(BlockStateWidget->WidgetTree->FindWidget("BGImage"));
			if (Background)
				Background->SetColorAndOpacity(Green);
		}
	}
	else if (StateName == "Run")
	{
		if (RunStateWidget)
		{
			// Change background color to green
			const auto Background = Cast<UImage>(RunStateWidget->WidgetTree->FindWidget("BGImage"));
			if (Background)
				Background->SetColorAndOpacity(Green);
		}
	}
	else
		ULog::LogDebugMessage(WARNING, "Nothing to highlight", true);
}

void UFSMVisualizerHUD::UpdateStateUptime(const FString& StateName, const float Uptime)
{
	if (StateName == "Idle")
	{
		if (IdleStateWidget)
		{
			const auto UptimeTextWidget = Cast<UTextBlock>(IdleStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
			{
				FNumberFormattingOptions NumberFormattingOptions;
				NumberFormattingOptions.MaximumFractionalDigits = 2;
				UptimeTextWidget->SetText(FText::AsNumber(Uptime, &NumberFormattingOptions));
			}
		}
	}
	else if (StateName == "Walk")
	{
		if (WalkStateWidget)
		{
			const auto UptimeTextWidget = Cast<UTextBlock>(WalkStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
			{
				FNumberFormattingOptions NumberFormattingOptions;
				NumberFormattingOptions.MaximumFractionalDigits = 2;
				UptimeTextWidget->SetText(FText::AsNumber(Uptime, &NumberFormattingOptions));
			}
		}
	}
	else if (StateName == "Jump")
	{
		if (JumpStateWidget)
		{
			const auto UptimeTextWidget = Cast<UTextBlock>(JumpStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
			{
				FNumberFormattingOptions NumberFormattingOptions;
				NumberFormattingOptions.MaximumFractionalDigits = 1;
				UptimeTextWidget->SetText(FText::AsNumber(Uptime, &NumberFormattingOptions));
			}
		}
	}
	else if (StateName == "Falling")
	{
		if (FallStateWidget)
		{
			const auto UptimeTextWidget = Cast<UTextBlock>(FallStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
			{
				FNumberFormattingOptions NumberFormattingOptions;
				NumberFormattingOptions.MaximumFractionalDigits = 2;
				UptimeTextWidget->SetText(FText::AsNumber(Uptime, &NumberFormattingOptions));
			}
		}
	}
	else if (StateName == "Attack")
	{
		if (AttackStateWidget)
		{
			const auto UptimeTextWidget = Cast<UTextBlock>(AttackStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
			{
				FNumberFormattingOptions NumberFormattingOptions;
				NumberFormattingOptions.MaximumFractionalDigits = 2;
				UptimeTextWidget->SetText(FText::AsNumber(Uptime, &NumberFormattingOptions));
			}
		}
	}
	else if (StateName == "Block")
	{
		if (BlockStateWidget)
		{
			const auto UptimeTextWidget = Cast<UTextBlock>(BlockStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
			{
				FNumberFormattingOptions NumberFormattingOptions;
				NumberFormattingOptions.MaximumFractionalDigits = 2;
				UptimeTextWidget->SetText(FText::AsNumber(Uptime, &NumberFormattingOptions));
			}
		}
	}
	else if (StateName == "Run")
	{
		if (RunStateWidget)
		{
			const auto UptimeTextWidget = Cast<UTextBlock>(RunStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
			{
				FNumberFormattingOptions NumberFormattingOptions;
				NumberFormattingOptions.MaximumFractionalDigits = 2;
				UptimeTextWidget->SetText(FText::AsNumber(Uptime, &NumberFormattingOptions));
			}
		}
	}
}

void UFSMVisualizerHUD::UnhighlightState(const FString& StateName)
{
	if (StateName == "Idle")
	{
		if (IdleStateWidget)
		{
			// Change background color to green
			const auto Background = Cast<UImage>(IdleStateWidget->WidgetTree->FindWidget("BGImage"));
			if (Background)
				Background->SetColorAndOpacity(White);

			// Set uptime text back to 0
			const auto UptimeTextWidget = Cast<UTextBlock>(IdleStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
				UptimeTextWidget->SetText(FText::AsNumber(0));
		}
	}
	else if (StateName == "Walk")
	{
		if (WalkStateWidget)
		{
			// Change background color to green
			const auto Background = Cast<UImage>(WalkStateWidget->WidgetTree->FindWidget("BGImage"));
			if (Background)
				Background->SetColorAndOpacity(White);

			// Set uptime text back to 0
			const auto UptimeTextWidget = Cast<UTextBlock>(WalkStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
				UptimeTextWidget->SetText(FText::AsNumber(0));
		}
	}
	else if (StateName == "Jump")
	{
		if (JumpStateWidget)
		{
			// Change background color to green
			const auto Background = Cast<UImage>(JumpStateWidget->WidgetTree->FindWidget("BGImage"));
			if (Background)
				Background->SetColorAndOpacity(White);

			// Set uptime text back to 0
			const auto UptimeTextWidget = Cast<UTextBlock>(JumpStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
				UptimeTextWidget->SetText(FText::AsNumber(0));
		}
	}
	else if (StateName == "Falling")
	{
		if (FallStateWidget)
		{
			// Change background color to green
			const auto Background = Cast<UImage>(FallStateWidget->WidgetTree->FindWidget("BGImage"));
			if (Background)
				Background->SetColorAndOpacity(White);

			// Set uptime text back to 0
			const auto UptimeTextWidget = Cast<UTextBlock>(FallStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
				UptimeTextWidget->SetText(FText::AsNumber(0));
		}
	}
	else if (StateName == "Attack")
	{
		if (AttackStateWidget)
		{
			// Change background color to green
			const auto Background = Cast<UImage>(AttackStateWidget->WidgetTree->FindWidget("BGImage"));
			if (Background)
				Background->SetColorAndOpacity(White);

			// Set uptime text back to 0
			const auto UptimeTextWidget = Cast<UTextBlock>(AttackStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
				UptimeTextWidget->SetText(FText::AsNumber(0));
		}
	}
	else if (StateName == "Block")
	{
		if (BlockStateWidget)
		{
			// Change background color to green
			const auto Background = Cast<UImage>(BlockStateWidget->WidgetTree->FindWidget("BGImage"));
			if (Background)
				Background->SetColorAndOpacity(White);

			// Set uptime text back to 0
			const auto UptimeTextWidget = Cast<UTextBlock>(BlockStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
				UptimeTextWidget->SetText(FText::AsNumber(0));
		}
	}
	else if (StateName == "Run")
	{
		if (RunStateWidget)
		{
			// Change background color to green
			const auto Background = Cast<UImage>(RunStateWidget->WidgetTree->FindWidget("BGImage"));
			if (Background)
				Background->SetColorAndOpacity(White);

			// Set uptime text back to 0
			const auto UptimeTextWidget = Cast<UTextBlock>(RunStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
				UptimeTextWidget->SetText(FText::AsNumber(0));
		}
	}
	else
		ULog::LogDebugMessage(WARNING, "Nothing to Un-highlight", true);
}
