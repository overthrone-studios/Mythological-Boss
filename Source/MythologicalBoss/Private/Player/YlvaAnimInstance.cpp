// Copyright Overthrone Studios 2019

#include "Player/YlvaAnimInstance.h"

#include "Player/Ylva.h"

void UYlvaAnimInstance::LeaveAllStates()
{
	Super::LeaveAllStates();

	bIsBlocking = false;
	bIsShieldHit = false;
}

void UYlvaAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Ylva = Cast<AYlva>(TryGetPawnOwner());

	StateMachines[0] = GetStateMachineIndex("Generics");

	ActiveStateMachine = StateMachines[0];
}
