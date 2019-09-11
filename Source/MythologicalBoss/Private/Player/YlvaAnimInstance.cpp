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

	ActiveStateMachine = StateMachines[0];
}
