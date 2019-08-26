// Copyright Overthrone Studios 2019

#include "Widgets/HUD/MasterHUD.h"
#include "WidgetTree.h"
#include "WidgetSwitcher.h"
#include "Log.h"
#include "Image.h"
#include "TextBlock.h"

static FLinearColor GWhite = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
static FLinearColor GGreen = FLinearColor(0.187166f, 1.0f, 0.198517f, 1.0f);

void UMasterHUD::Init()
{
	Super::Init();

	DebugOptions = Cast<UPanelWidget>(WidgetTree->FindWidget(FName("DebugOptions")));
	Title = Cast<UTextBlock>(WidgetTree->FindWidget(FName("Title")));

	// Store our debug boxes for un/highlighting
	StoreAllDebugOptions(DebugOptions);

	// Get the widget switcher
	WidgetSwitcher = Cast<UWidgetSwitcher>(WidgetTree->FindWidget(FName("HUDSwitcher")));

	// Get every HUD widget in this HUD
	HUDs = GetAllChildHUDs();

	// Initialize all our HUDs
	for (auto HUD : HUDs)
	{
		HUD->Init();

		if (bLogHUDInitialized)
			ULog::DebugMessage(INFO, FString(HUD->GetName() + " initialized"), true);
	}

	// Set debug box background to green to represent the active HUD
	HighlightBox(0);
}

void UMasterHUD::HighlightBox(const int32 Index)
{
	if (Index > Boxes.Num() - 1)
	{
		ULog::DebugMessage(ERROR, "Index " + FString::FromInt(Index) + " is out of array bounds!", true);
		return;
	}

	const auto Background = Cast<UImage>(Boxes[Index]->WidgetTree->FindWidget("BGImage"));
	Background->SetColorAndOpacity(GGreen);
}

void UMasterHUD::UnhighlightBox(const int32 Index)
{
	if (Index > Boxes.Num() - 1)
	{
		ULog::DebugMessage(ERROR, "Index " + FString::FromInt(Index) + " is out of array bounds!", true);
		return;
	}

	const auto Background = Cast<UImage>(Boxes[Index]->WidgetTree->FindWidget("BGImage"));
	Background->SetColorAndOpacity(GWhite);
}

void UMasterHUD::ShowTitle()
{
	Title->SetVisibility(ESlateVisibility::Visible);
}

void UMasterHUD::HideTitle()
{
	Title->SetVisibility(ESlateVisibility::Hidden);
}

void UMasterHUD::ShowBoxes()
{
	DebugOptions->SetVisibility(ESlateVisibility::Visible);
}

void UMasterHUD::HideBoxes()
{
	DebugOptions->SetVisibility(ESlateVisibility::Hidden);
}

UHUDBase* UMasterHUD::GetHUD(const TSubclassOf<UHUDBase> HUDClass) const
{
	for (auto HUD : HUDs)
	{
		if (HUD->IsA(HUDClass))
			return HUD;
	}

	return nullptr;
}

UHUDBase* UMasterHUD::GetHUD(const FString& HUDWidgetName) const
{
	for (auto HUD : HUDs)
	{
		if (HUD->GetName() == HUDWidgetName)
			return HUD;
	}

	return nullptr;
}

TArray<UHUDBase*> UMasterHUD::GetAllHUDs() const
{
	return HUDs;
}

void UMasterHUD::StoreAllDebugOptions(const UPanelWidget* ParentWidget)
{
	for (int32 i = 0; i < ParentWidget->GetChildrenCount(); i++)
	{
		const auto Widget = ParentWidget->GetChildAt(i);

		if (Widget->IsA(UPanelWidget::StaticClass()))
			StoreAllDebugOptions(Cast<UPanelWidget>(Widget));
		else
			Boxes.Add(Cast<UUserWidget>(Widget));
	}
}

UWidget* UMasterHUD::GetActiveHUDWidget() const
{
	return WidgetSwitcher->GetActiveWidget();
}

int32 UMasterHUD::GetActiveHUDIndex() const
{
	return WidgetSwitcher->GetActiveWidgetIndex();
}

void UMasterHUD::SwitchToHUD(UHUDBase* HUD)
{
	UnhighlightBox(GetActiveHUDIndex());

	WidgetSwitcher->SetActiveWidget(HUD);

	HighlightBox(GetActiveHUDIndex());

	ShowTitle();
	ShowBoxes();
	SetVisibility(ESlateVisibility::Visible);
}

void UMasterHUD::SwitchToHUDIndex(const int32 Index)
{
	UnhighlightBox(GetActiveHUDIndex());

	WidgetSwitcher->SetActiveWidgetIndex(Index);

	HighlightBox(Index);

	ShowTitle();
	ShowBoxes();
	SetVisibility(ESlateVisibility::Visible);
}

TArray<UHUDBase*> UMasterHUD::GetAllChildHUDs() const
{
	TArray<UHUDBase*> FoundHUDs;

	for (int32 i = 0; i < WidgetSwitcher->GetChildrenCount(); i++)
	{
		const auto HUD = WidgetSwitcher->GetChildAt(i);

		if (HUD->IsA(UHUDBase::StaticClass()))
			FoundHUDs.Add(Cast<UHUDBase>(HUD));
	}

	return FoundHUDs;
}
