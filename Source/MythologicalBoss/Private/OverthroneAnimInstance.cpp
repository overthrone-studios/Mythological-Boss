// Copyright Overthrone Studios 2019

#include "OverthroneAnimInstance.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Log.h"

void UOverthroneAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Cache the owning pawn for use in Tick
	OwningPawn = TryGetPawnOwner();

	// Cache the owning pawn's movement component for use in Tick
	if (OwningPawn)
	{
		PawnMovementComponent = OwningPawn->GetMovementComponent();
	}
}

void UOverthroneAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!OwningPawn || !PawnMovementComponent)
		return;

	const FVector Velocity = OwningPawn->GetVelocity();

	// Update movement speed and direction for use in the locomotion blendspace
	MovementSpeed = Velocity.Size();

	if (bLogDirection)
	{
		ULog::Number(MovementDirection, "Direction: ", true);
	}

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

	const int32 StateIndex = StateMachine->GetCurrentState();

	return GetRelevantAnimTimeRemaining(ActiveStateMachine, StateIndex);
}
