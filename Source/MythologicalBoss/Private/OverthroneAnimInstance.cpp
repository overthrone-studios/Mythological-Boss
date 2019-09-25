// Copyright Overthrone Studios 2019

#include "OverthroneAnimInstance.h"
#include "Animation/AnimNode_StateMachine.h"

void UOverthroneAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	AnimTimeRemaining = GetAnimTimeRemaining();
}

void UOverthroneAnimInstance::LeaveAllStates()
{
	bIsHit = false;
	bIsDead = false;
	bAcceptLightAttack = false;
	bAcceptSecondLightAttack = false;
	bAcceptThirdLightAttack = false;
	bAcceptHeavyAttack = false;
	bAcceptSecondHeavyAttack = false;
	bAcceptThirdHeavyAttack = false;
}

float UOverthroneAnimInstance::GetAnimTimeRemaining()
{
	const auto StateMachine = GetStateMachineInstance(ActiveStateMachine);

	if (!StateMachine)
		return 0.0f;

	ActiveStateIndex = StateMachine->GetCurrentState();

	return GetRelevantAnimTimeRemaining(ActiveStateMachine, ActiveStateIndex);
}
