// Copyright Overthrone Studios 2019

#include "Widgets/HUD/MasterHUD.h"
#include "WidgetTree.h"
#include "WidgetSwitcher.h"
#include "Log.h"
#include "Image.h"

static FLinearColor White = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
static FLinearColor Green = FLinearColor(0.187166f, 1.0f, 0.198517f, 1.0f);

void UMasterHUD::Init()
{
	Super::Init();

	// Get the boxes in this HUD
	DebugBox = Cast<UUserWidget>(WidgetTree->FindWidget(FName("DebugBox")));
	MainBox = Cast<UUserWidget>(WidgetTree->FindWidget(FName("MainHUDBox")));
	NoHUDBox = Cast<UUserWidget>(WidgetTree->FindWidget(FName("NoHUDBox")));

	// Get the widget switcher
	WidgetSwitcher = Cast<UWidgetSwitcher>(WidgetTree->FindWidget(FName("HUDSwitcher")));

	// Get every HUD widget in this HUD
	HUDs = GetAllChildHUDs();

	// Initialize them all
	for (auto HUD : HUDs)
	{
		HUD->Init();
		ULog::LogDebugMessage(INFO, FString(HUD->GetName() + " initialized"), true);
	}

	// Set debug box background to green to represent the active HUD
	HighlightBox(0);
}

void UMasterHUD::HighlightBox(const int32 Index)
{
	switch (Index)
	{
		case 0:
		{
			const auto Background = Cast<UImage>(DebugBox->WidgetTree->FindWidget("BGImage"));
			Background->SetColorAndOpacity(Green);
		}
		break;
		
		case 1:
		{
			const auto Background = Cast<UImage>(MainBox->WidgetTree->FindWidget("BGImage"));
			Background->SetColorAndOpacity(Green);
		}
		break;
		
		case 2:
		{
			const auto Background = Cast<UImage>(NoHUDBox->WidgetTree->FindWidget("BGImage"));
			Background->SetColorAndOpacity(Green);

			SetVisibility(ESlateVisibility::Hidden);

			return;
		}

		default:
		break;
	}

	SetVisibility(ESlateVisibility::Visible);
}

void UMasterHUD::UnhighlightBox(const int32 Index)
{
	switch (Index)
	{
		case 0:
		{
			const auto Background = Cast<UImage>(DebugBox->WidgetTree->FindWidget("BGImage"));
			Background->SetColorAndOpacity(White);
		}
		break;
		
		case 1:
		{
			const auto Background = Cast<UImage>(MainBox->WidgetTree->FindWidget("BGImage"));
			Background->SetColorAndOpacity(White);
		}
		break;
		
		case 2:
		{
			const auto Background = Cast<UImage>(NoHUDBox->WidgetTree->FindWidget("BGImage"));
			Background->SetColorAndOpacity(White);
		}
		break;

		default:
		break;
	}
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
}

void UMasterHUD::SwitchToHUDIndex(const int32 Index)
{
	UnhighlightBox(GetActiveHUDIndex());

	WidgetSwitcher->SetActiveWidgetIndex(Index);

	HighlightBox(Index);
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
