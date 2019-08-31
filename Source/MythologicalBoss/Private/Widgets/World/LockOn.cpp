// Copyright Overthrone Studios 2019

#include "LockOn.h"

#include "Public/OverthroneGameInstance.h"

#include "Widgets/World/LockOnWidget.h"

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
			LockOnWidgetComponent = Cast<UWidgetComponent>(Component);
			LockOnWidget = Cast<ULockOnWidget>(LockOnWidgetComponent->GetUserWidgetObject());

			LockOnWidgetComponent->SetHiddenInGame(true);

			break;
		}
	}
}

void ALockOn::Show()
{
	LockOnWidget->PlayAnimation(LockOnWidget->Fade, 0.0f, 1, EUMGSequencePlayMode::Forward, 2.0f);
	LockOnWidgetComponent->SetHiddenInGame(false);
}

void ALockOn::Hide()
{
	LockOnWidget->PlayAnimation(LockOnWidget->Fade, 0.0f, 1, EUMGSequencePlayMode::Reverse, 2.0f);
}

void ALockOn::ToggleVisibility(const bool bVisibility)
{
	bVisibility ? Hide() : Show();
}
