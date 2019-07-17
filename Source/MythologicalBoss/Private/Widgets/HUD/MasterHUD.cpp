// Copyright Overthrone Studios 2019

#include "Widgets/HUD/MasterHUD.h"
#include "WidgetTree.h"
#include "WidgetSwitcher.h"
#include "Log.h"

void UMasterHUD::Init()
{
	Super::Init();

	// Get the widget switcher
	WidgetSwitcher = Cast<UWidgetSwitcher>(WidgetTree->FindWidget(FName("HUDSwitcher")));

	HUDs = GetAllChildHUDs();

	for (auto HUD : HUDs)
	{
		HUD->Init();
		ULog::LogDebugMessage(INFO, FString(HUD->GetName() + " initialized"), true);
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
	WidgetSwitcher->SetActiveWidget(HUD);
}

void UMasterHUD::SwitchToHUDIndex(const int32 Index)
{
	WidgetSwitcher->SetActiveWidgetIndex(Index);
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
