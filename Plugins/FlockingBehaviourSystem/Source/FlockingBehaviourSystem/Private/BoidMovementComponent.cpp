// Copyright Ali El Saleh, 2019

#include "BoidMovementComponent.h"
#include "Boid.h"

UBoidMovementComponent::UBoidMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBoidMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ABoid>(GetOwner());

	// Generate a random velocity vector to get the boid moving
	FVector RandomVelocity;
	RandomVelocity.X = FMath::FRandRange(-1000.0f, 1000.0f);
	RandomVelocity.Y = FMath::FRandRange(-1000.0f, 1000.0f);
	RandomVelocity.Z = FMath::FRandRange(-1000.0f, 1000.0f);
	Velocity = RandomVelocity;

	// Default settings
	MaxSpeed = Owner->GetSpeed();
	TurningBoost = Owner->GetTurnSpeed();
	SetUpdateNavAgentWithOwnersCollisions(false);
}
