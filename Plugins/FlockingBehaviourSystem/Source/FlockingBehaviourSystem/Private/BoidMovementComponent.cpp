// Copyright Ali El Saleh, 2019

#include "BoidMovementComponent.h"
#include "Boid.h"

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

void UBoidMovementComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Set the boids' new rotation, after we've calculated the steering vector
	Owner->UpdateRotation(DeltaTime);
	//Owner->Debug();
}
