// Copyright Overthrone Studios 2019

#include "Public/PlayerHUD.h"
#include "Widget.h"
#include "WidgetTree.h"
#include "UserWidget.h"
#include "Image.h"
#include "TextBlock.h"
#include "ConstructorHelpers.h"
#include "Log.h"

static FLinearColor White = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
static FLinearColor Green = FLinearColor(0.187166f, 1.0f, 0.198517f, 1.0f);

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FClassFinder<UWidget> FSMVisualizer(TEXT("WidgetBlueprint'/Game/UI/Debug/FSM/UI_FSMVisualizer.UI_FSMVisualizer_C'"));

	if (FSMVisualizer.Succeeded())
		FSMVisualizerClass = FSMVisualizer.Class;
}

void APlayerHUD::Init()
{
	CreateWidgets();
	AddWidgetsToScreen();
	InitWidgets();
}

void APlayerHUD::HighlightState(const FString& StateName)
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
	else if (StateName == "Fall")
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
	else
		ULog::LogDebugMessage(WARNING, "Nothing to highlight", true);
}

void APlayerHUD::UpdateStateUptime(const FString& StateName, const float Uptime)
{
	if (StateName == "Idle")
	{
		if (IdleStateWidget)
		{
			const auto UptimeTextWidget = Cast<UTextBlock>(IdleStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
				UptimeTextWidget->SetText(FText::AsNumber(Uptime));
		}
	}
	else if (StateName == "Walk")
	{
		if (WalkStateWidget)
		{
			const auto UptimeTextWidget = Cast<UTextBlock>(WalkStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
				UptimeTextWidget->SetText(FText::AsNumber(Uptime));
		}
	}
	else if (StateName == "Jump")
	{
		if (JumpStateWidget)
		{
			const auto UptimeTextWidget = Cast<UTextBlock>(JumpStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
				UptimeTextWidget->SetText(FText::AsNumber(Uptime));
		}
	}
	else if (StateName == "Fall")
	{
		if (FallStateWidget)
		{
			const auto UptimeTextWidget = Cast<UTextBlock>(FallStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
				UptimeTextWidget->SetText(FText::AsNumber(Uptime));
		}
	}
	else if (StateName == "Attack")
	{
		if (AttackStateWidget)
		{
			const auto UptimeTextWidget = Cast<UTextBlock>(AttackStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
				UptimeTextWidget->SetText(FText::AsNumber(Uptime));
		}
	}
	else if (StateName == "Block")
	{
		if (BlockStateWidget)
		{
			const auto UptimeTextWidget = Cast<UTextBlock>(BlockStateWidget->WidgetTree->FindWidget("Uptime"));
			if (UptimeTextWidget)
				UptimeTextWidget->SetText(FText::AsNumber(Uptime));
		}
	}
}

void APlayerHUD::UnhighlightState(const FString& StateName)
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
	else if (StateName == "Fall")
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
	else
		ULog::LogDebugMessage(WARNING, "Nothing to Un-highlight", true);
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerHUD::CreateWidgets()
{
	FSMVisualizer = CreateWidget<UUserWidget>(GetWorld(), FSMVisualizerClass, FName("FSM Visualizer"));
}

void APlayerHUD::AddWidgetsToScreen()
{
	FSMVisualizer->AddToViewport();
	FSMVisualizer->SetVisibility(ESlateVisibility::Visible);
}

void APlayerHUD::InitWidgets()
{
	IdleStateWidget = Cast<UUserWidget>(FSMVisualizer->WidgetTree->FindWidget("IdleState"));
	WalkStateWidget = Cast<UUserWidget>(FSMVisualizer->WidgetTree->FindWidget("WalkState"));
	JumpStateWidget = Cast<UUserWidget>(FSMVisualizer->WidgetTree->FindWidget("JumpState"));
	FallStateWidget = Cast<UUserWidget>(FSMVisualizer->WidgetTree->FindWidget("FallState"));
	BlockStateWidget = Cast<UUserWidget>(FSMVisualizer->WidgetTree->FindWidget("BlockState"));
	AttackStateWidget = Cast<UUserWidget>(FSMVisualizer->WidgetTree->FindWidget("AttackState"));
}
