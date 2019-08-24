// Copyright Overthrone Studios 2019

#include "MordathAnimInstance.h"

void UMordathAnimInstance::LeaveAllStates()
{
	Super::LeaveAllStates();

	bCanLaugh = false;
	bIsBeaten = false;
	bIsStunned = false;
}

void UMordathAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	StateMachines[0] = GetStateMachineIndex("Stage 1");
	StateMachines[1] = GetStateMachineIndex("Stage 2");
	StateMachines[2] = GetStateMachineIndex("Stage 3");
}
