// Copyright Overthrone Studios 2019

#include "LockOn.h"

#include "OverthroneFunctionLibrary.h"
#include "OverthroneGameState.h"

#include "Widgets/World/LockOnWidget.h"

#include "Components/WidgetComponent.h"

#include "Log.h"

ALockOn::ALockOn()
{
	PrimaryActorTick.bCanEverTick = true;

	OnToggleLockOn.AddDynamic(this, &ALockOn::ToggleVisibility);
}

void ALockOn::BeginPlay()
{
	Super::BeginPlay();

	GameState = UOverthroneFunctionLibrary::GetGameState(this);
	GameState->LockOn = this;

	// Get our lock on widget component
	const auto Components = GetComponents();
	for (auto Component : Components)
	{
		if (Component->IsA(UWidgetComponent::StaticClass()))
		{
			LockOnWidgetComponent = Cast<UWidgetComponent>(Component);
			LockOnWidget = Cast<ULockOnWidget>(LockOnWidgetComponent->GetUserWidgetObject());

			LockOnWidget->RenderOpacity = 0.0f;

			LockOnWidgetComponent->PrimaryComponentTick.bCanEverTick = false;

			break;
		}
	}
}

void ALockOn::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetActorLocation(GameState->LockOnLocation);
}

void ALockOn::Show()
{
	if (LockOnWidget && LockOnWidget->Fade)
		LockOnWidget->PlayAnimation(LockOnWidget->Fade, 0.0f, 1, EUMGSequencePlayMode::Forward, 2.0f);
}

void ALockOn::Hide()
{
	if (LockOnWidget && LockOnWidget->Fade)
		LockOnWidget->PlayAnimation(LockOnWidget->Fade, 0.0f, 1, EUMGSequencePlayMode::Reverse, 2.0f);
}

void ALockOn::ToggleVisibility(const bool bVisibility)
{
	bVisibility ? Hide() : Show();
}
