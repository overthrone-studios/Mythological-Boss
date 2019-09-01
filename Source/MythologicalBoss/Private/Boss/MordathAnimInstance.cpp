// Copyright Overthrone Studios 2019

#include "MordathAnimInstance.h"

void UMordathAnimInstance::LeaveAllStates()
{
	Super::LeaveAllStates();

	bCanLaugh = false;
	bIsBeaten = false;
	bIsStunned = false;
	bCanTeleport = false;
}

void UMordathAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	StateMachines[0] = GetStateMachineIndex("Stage 1");
	StateMachines[1] = GetStateMachineIndex("Stage 2");
	StateMachines[2] = GetStateMachineIndex("Stage 3");
}

void UMordathAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwningPawn || !PawnMovementComponent)
		return;

	const FVector Velocity = OwningPawn->GetVelocity();

	MovementDirection = CalculateDirection(Velocity, OwningPawn->GetControlRotation());
}
