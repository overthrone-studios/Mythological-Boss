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
}
