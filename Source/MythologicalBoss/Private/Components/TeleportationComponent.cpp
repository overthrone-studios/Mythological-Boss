// Copyright Overthrone Studios 2019

#include "TeleportationComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

UTeleportationComponent::UTeleportationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTeleportationComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
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

	if (!InBox.IsInside(NewLocation))
	{
		NewLocation = FMath::RandPointInBox(InBox);

		return FVector(NewLocation.X, NewLocation.Y, Owner->GetActorLocation().Z);
	}

	if (bShowTeleportedLocation)
		DrawDebugSphere(GetWorld(), FVector(NewLocation.X, NewLocation.Y, Origin.Z), 20.0f, 20, FColor::Green, false, 2.0f, 0.0f, 3.0f);

	return NewLocation;
}
