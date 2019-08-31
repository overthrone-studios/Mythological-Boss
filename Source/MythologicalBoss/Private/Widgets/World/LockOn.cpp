// Copyright Overthrone Studios 2019

#include "LockOn.h"

#include "Public/OverthroneGameInstance.h"

#include "Components/WidgetComponent.h"

#include "Engine/World.h"

ALockOn::ALockOn()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALockOn::BeginPlay()
{
	Super::BeginPlay();

	class UOverthroneGameInstance* GameInstance = Cast<UOverthroneGameInstance>(GetWorld()->GetGameInstance());
	GameInstance->LockOn = this;

	// Get our lock on widget component
	const auto Components = GetComponents();
	for (auto Component : Components)
	{
		if (Component->IsA(UWidgetComponent::StaticClass()))
		{
			LockOnWidget = Cast<UWidgetComponent>(Component);
			Hide();
			break;
		}
	}
}

void ALockOn::Show()
{
	LockOnWidget->SetHiddenInGame(false);
}

void ALockOn::Hide()
{
	LockOnWidget->SetHiddenInGame(true);
}

void ALockOn::ToggleVisibility(const bool bVisibility)
{
	LockOnWidget->SetHiddenInGame(bVisibility);
}
