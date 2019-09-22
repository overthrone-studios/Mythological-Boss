// Copyright Overthrone Studios 2019

#include "TeleportationComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Log.h"
#include "TimerManager.h"

UTeleportationComponent::UTeleportationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTeleportationComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	TimerManager = &Owner->GetWorldTimerManager();
}

void UTeleportationComponent::GenerateTeleportTime()
{
	const float Min = TeleportTime - RandomDeviation;
	const float Max = TeleportTime + RandomDeviation;
	const float NewTime = FMath::FRandRange(Min, Max);

	TeleportTime = NewTime;

	if (bLogTeleportTime)
		ULog::Number(TeleportTime, "Teleport time: ", true);
}

void UTeleportationComponent::StartCooldown()
{
	TimerManager->SetTimer(TH_Cooldown, CooldownTime, false);
}

bool UTeleportationComponent::IsCoolingDown()
{
	return TimerManager->IsTimerActive(TH_Cooldown);
}

FVector UTeleportationComponent::FindLocationToTeleport(const FVector& Origin, const float Radius, const FBox& InBox) const
{
	const float T = FMath::RandRange(PI, 2 * PI);

	FVector NewLocation
	{
		Origin.X + Radius * FMath::Cos(T), 
		Origin.Y + Radius * FMath::Sin(T), 
		Owner->GetActorLocation().Z
	};

	// If the calculated location is outside the box
	if (!InBox.IsInside(NewLocation))
	{
		NewLocation = FMath::RandPointInBox(InBox);

		return FVector(NewLocation.X, NewLocation.Y, Owner->GetActorLocation().Z);
	}

	if (bShowTeleportedLocation)
		DrawDebugSphere(GetWorld(), FVector(NewLocation.X, NewLocation.Y, Origin.Z), 20.0f, 20, FColor::Green, false, 2.0f, 0.0f, 3.0f);

	return NewLocation;
}
