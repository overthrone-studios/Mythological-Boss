// Copyright Overthrone Studios 2019

#include "Widgets/HUD/MasterHUD.h"
#include "WidgetTree.h"
#include "WidgetSwitcher.h"
#include "Log.h"
#include "Image.h"
#include "TextBlock.h"

static FLinearColor White = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
static FLinearColor Green = FLinearColor(0.187166f, 1.0f, 0.198517f, 1.0f);

void UMasterHUD::Init()
{
	Super::Init();

	HUDOptions = Cast<UPanelWidget>(WidgetTree->FindWidget(FName("HUDoptions")));
	Title = Cast<UTextBlock>(WidgetTree->FindWidget(FName("Title")));

	// Get the boxes
	Boxes.Reserve(HUDOptions->GetChildrenCount());
	for (int32 i = 0; i < HUDOptions->GetChildrenCount(); i++)
	{
		const auto Widget = Cast<UUserWidget>(HUDOptions->GetChildAt(i));
		if (Widget)
			Boxes.Add(Widget);
		else
			ULog::DebugMessage(ERROR, HUDOptions->GetChildAt(i)->GetName() + " is not a user widget", true);
	}

	// Get the widget switcher
	WidgetSwitcher = Cast<UWidgetSwitcher>(WidgetTree->FindWidget(FName("HUDSwitcher")));

	// Get every HUD widget in this HUD
	HUDs = GetAllChildHUDs();

	// Initialize them all
	for (auto HUD : HUDs)
	{
		HUD->Init();
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
	Background->SetColorAndOpacity(Green);
}

void UMasterHUD::UnhighlightBox(const int32 Index)
{
	if (Index > Boxes.Num() - 1)
	{
		ULog::DebugMessage(ERROR, "Index " + FString::FromInt(Index) + " is out of array bounds!", true);
		return;
	}

	const auto Background = Cast<UImage>(Boxes[Index]->WidgetTree->FindWidget("BGImage"));
	Background->SetColorAndOpacity(White);
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
	HUDOptions->SetVisibility(ESlateVisibility::Visible);
}

void UMasterHUD::HideBoxes()
{
	HUDOptions->SetVisibility(ESlateVisibility::Hidden);
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
