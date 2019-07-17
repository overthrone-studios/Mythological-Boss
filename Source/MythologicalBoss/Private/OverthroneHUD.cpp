// Copyright Overthrone Studios 2019

#include "Public/OverthroneHUD.h"
#include "Widgets/HUD/MasterHUD.h"
#include "Widget.h"
#include "WidgetTree.h"
#include "UserWidget.h"
#include "Image.h"
#include "TextBlock.h"
#include "ConstructorHelpers.h"
#include "Log.h"

static FLinearColor White = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
static FLinearColor Green = FLinearColor(0.187166f, 1.0f, 0.198517f, 1.0f);

AOverthroneHUD::AOverthroneHUD()
{
	static ConstructorHelpers::FClassFinder<UWidget> MasterHUDWidget(TEXT("WidgetBlueprint'/Game/UI/PlayerHUD/UI_MasterPlayerHUD.UI_MasterPlayerHUD_C'"));

	if (MasterHUDWidget.Succeeded())
		WidgetClass = MasterHUDWidget.Class;
}

void AOverthroneHUD::Init()
{
	CreateWidgets();

	if (MasterHUD)
	{
		AddWidgetsToScreen();
		InitWidgets();
	}
	else
	{
		ULog::LogDebugMessage(ERROR, "Failed to create the MasterHUD widget!", true);
	}
}

void AOverthroneHUD::HighlightState(const FString& StateName)
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

void AOverthroneHUD::UpdateStateUptime(const FString& StateName, const float Uptime)
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

void AOverthroneHUD::UnhighlightState(const FString& StateName)
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

void AOverthroneHUD::BeginPlay()
{
	Super::BeginPlay();

}

void AOverthroneHUD::CreateWidgets()
{
	MasterHUD = CreateWidget<UMasterHUD>(GetWorld(), WidgetClass, FName("Master Player HUD"));
}

void AOverthroneHUD::AddWidgetsToScreen()
{
	MasterHUD->AddToViewport();
	MasterHUD->SetVisibility(ESlateVisibility::Visible);
}

void AOverthroneHUD::InitWidgets()
{
	MasterHUD->Init();

	/*IdleStateWidget = Cast<UUserWidget>(MasterHUD->WidgetTree->FindWidget("IdleState"));
	WalkStateWidget = Cast<UUserWidget>(MasterHUD->WidgetTree->FindWidget("WalkState"));
	JumpStateWidget = Cast<UUserWidget>(MasterHUD->WidgetTree->FindWidget("JumpState"));
	FallStateWidget = Cast<UUserWidget>(MasterHUD->WidgetTree->FindWidget("FallState"));
	BlockStateWidget = Cast<UUserWidget>(MasterHUD->WidgetTree->FindWidget("BlockState"));
	AttackStateWidget = Cast<UUserWidget>(MasterHUD->WidgetTree->FindWidget("AttackState"));*/
}
