// Copyright Overthrone Studios 2019

#include "FloatingComponent.h"
#include "GameFramework/Actor.h"


UFloatingComponent::UFloatingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Offset = FVector(0.0f, 0.0f, 50.0f);
	Speed = 1.0f;
}

void UFloatingComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
}

void UFloatingComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update location every frame to create the floating effect
	FVector NewLocation = Owner->GetActorLocation();

	const float DeltaHeight = FMath::Sin(RunningTime + Speed + DeltaTime) - FMath::Sin(RunningTime + Speed);

	NewLocation.X += DeltaHeight * Offset.X;
	NewLocation.Y += DeltaHeight * Offset.Y;
	NewLocation.Z += DeltaHeight * Offset.Z;

	RunningTime += DeltaTime * Speed;

	Owner->SetActorLocation(NewLocation);
}
