// Copyright Overthrone Studios 2019

#include "MordathAnimInstance.h"

void UMordathAnimInstance::LeaveAllStates()
{
	Super::LeaveAllStates();

	bCanLaugh = false;
	bIsBeaten = false;
	bIsStunned = false;
}
