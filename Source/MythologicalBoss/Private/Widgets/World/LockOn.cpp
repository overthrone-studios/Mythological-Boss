// Copyright Overthrone Studios 2019

#include "LockOn.h"

#include "Public/OverthroneGameInstance.h"

#include "Widgets/World/LockOnWidget.h"

#include "Components/WidgetComponent.h"

#include "Engine/World.h"
#include "Log.h"

ALockOn::ALockOn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALockOn::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UOverthroneGameInstance>(GetWorld()->GetGameInstance());
	GameInstance->LockOn = this;

	// Get our lock on widget component
	const auto Components = GetComponents();
	for (auto Component : Components)
	{
		if (Component->IsA(UWidgetComponent::StaticClass()))
		{
			LockOnWidgetComponent = Cast<UWidgetComponent>(Component);
			LockOnWidget = Cast<ULockOnWidget>(LockOnWidgetComponent->GetUserWidgetObject());

			LockOnWidget->RenderOpacity = 0.0f;

			break;
		}
	}
}

void ALockOn::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetActorLocation(GameInstance->BossData.LockOnBoneLocation);
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
