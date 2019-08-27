// Copyright Overthrone Studios 2019

#include "RotatingComponent.h"
#include "GameFramework/Actor.h"


URotatingComponent::URotatingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	RotationRate = FVector(0.0f, 0.0f, 100.0);
}

void URotatingComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
}

void URotatingComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Rotate at the given rate
	FRotator NewRotation = Owner->GetActorRotation();

	NewRotation.Roll += DeltaTime * RotationRate.X;
	NewRotation.Pitch += DeltaTime * RotationRate.Y;
	NewRotation.Yaw += DeltaTime * RotationRate.Z;

	Owner->SetActorRotation(NewRotation);
}
