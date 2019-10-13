// Copyright Overthrone Studios 2019

#include "TeleportationComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Log.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "BoundingBox.h"

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

FVector UTeleportationComponent::FindLocationToTeleport(const FVector& Origin, const float Radius, const ABoundingBox* InPlayArea) const
{
	const float T = FMath::RandRange(0.0f, 2 * PI);

	FVector NewLocation
	{
		Origin.X + Radius * FMath::Cos(T), 
		Origin.Y + Radius * FMath::Sin(T), 
		Owner->GetActorLocation().Z
	};

	if (bShowTeleportedLocation)
	{
		// New teleported location
		DrawDebugLine(GetWorld(), NewLocation, NewLocation * FVector(1.0f, 1.0f, 3000.0f), FColor::Red, true, 5.0f, 0, 2.0f);

		// The origin location
		DrawDebugLine(GetWorld(), Origin, Origin * FVector(1.0f, 1.0f, 3000.0f), FColor::Green, true, 5.0f, 0, 2.0f);

		// The generated location visualized as a wire sphere
		DrawDebugSphere(GetWorld(), NewLocation, 20.0f, 20, FColor::Green, false, 2.0f, 0.0f, 3.0f);
	}

	// If the calculated location is outside the box
	if (!UKismetMathLibrary::IsPointInBox(NewLocation, InPlayArea->GetActorLocation(), InPlayArea->GetBoundingBox().GetExtent()))
	{
		NewLocation = UKismetMathLibrary::RandomPointInBoundingBox(InPlayArea->GetActorLocation(), InPlayArea->GetBoundingBox().GetExtent());

		return FVector(NewLocation.X, NewLocation.Y, Owner->GetActorLocation().Z);
	}


	return NewLocation;
}
