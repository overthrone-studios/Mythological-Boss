// Copyright Overthrone Studios 2019

#include "OverthroneAnimInstance.h"
#include "Log.h"
#include "Kismet/KismetMathLibrary.h"

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

	GenericsMachineIndex = GetStateMachineIndex("Generics");
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
