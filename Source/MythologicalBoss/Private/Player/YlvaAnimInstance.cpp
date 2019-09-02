// Copyright Overthrone Studios 2019

#include "Player/YlvaAnimInstance.h"

void UYlvaAnimInstance::LeaveAllStates()
{
	Super::LeaveAllStates();

	bIsBlocking = false;
	bIsShieldHit = false;
}

void UYlvaAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	StateMachines[0] = GetStateMachineIndex("Generics");
	StateMachines[1] = GetStateMachineIndex("Dash");

	ActiveStateMachine = StateMachines[0];
}
