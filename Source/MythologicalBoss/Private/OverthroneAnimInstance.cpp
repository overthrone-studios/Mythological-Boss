// Copyright Overthrone Studios 2019

#include "OverthroneAnimInstance.h"
#include "Public/OverthroneCharacter.h"

void UOverthroneAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Cache the owning pawn for use in Tick
	OwningPawn = TryGetPawnOwner();

	// Cache the owning pawn's movement component for use in Tick
	if (OwningPawn)
	{
		PawnMovementComponent = OwningPawn->GetMovementComponent();

		// If it's our character cache it for later use
		if (OwningPawn->IsA(AOverthroneCharacter::StaticClass()))
			OverthroneCharacter = Cast<AOverthroneCharacter>(OwningPawn);
	}

	GenericsMachineIndex = GetStateMachineIndex("Generics");
}

void UOverthroneAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!OwningPawn || !PawnMovementComponent)
		return;

	// Update movement speed for use in the Idle/Run blendspace
	MovementSpeed = OverthroneCharacter->GetVelocity().Size();
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
