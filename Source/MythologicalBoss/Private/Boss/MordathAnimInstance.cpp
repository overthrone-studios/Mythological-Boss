// Copyright Overthrone Studios 2019

#include "MordathAnimInstance.h"
#include "Mordath.h"

void UMordathAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Cache the owning pawn for use in Tick
	OwningPawn = TryGetPawnOwner();

	// Cache the owning pawn's movement component for use in Tick
	if (OwningPawn)
	{
		PawnMovementComponent = OwningPawn->GetMovementComponent();

		// If it's our character cache it for later use
		if (OwningPawn->IsA(AMordath::StaticClass()))
			Mordath = Cast<AMordath>(OwningPawn);
	}

	GenericsMachineIndex = GetStateMachineIndex("Generics");
}

void UMordathAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!OwningPawn || !PawnMovementComponent)
		return;

	// Update movement speed for use in the Idle/Run blendspace
	MovementSpeed = Mordath->GetVelocity().Size();
}
