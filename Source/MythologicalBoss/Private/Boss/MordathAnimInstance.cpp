// Copyright Overthrone Studios 2019

#include "MordathAnimInstance.h"
#include "Mordath.h"

void UMordathAnimInstance::LeaveAllStates()
{
	Super::LeaveAllStates();

	bCanRoar = false;
	bIsBeaten = false;
	bIsStunned = false;
	bCanTeleport = false;
}

void UMordathAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Mordath = Cast<AMordath>(TryGetPawnOwner());

	StateMachines[0] = GetStateMachineIndex("Stage 1");
	StateMachines[1] = GetStateMachineIndex("Stage 2");
}
