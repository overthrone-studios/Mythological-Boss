// Copyright Overthrone Studios 2019

#include "Player/YlvaAnimInstance.h"
#include "Player/Ylva.h"
#include "GameFramework/PawnMovementComponent.h"

UYlvaAnimInstance::UYlvaAnimInstance()
{
}

void UYlvaAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Cache the owning pawn for use in Tick
	OwningPawn = TryGetPawnOwner();

	// Cache the owning pawn's movement component for use in Tick
	if (OwningPawn)
	{
		PawnMovementComponent = OwningPawn->GetMovementComponent();

		// If it's our character cache it for later use
		if (OwningPawn->IsA(AYlva::StaticClass()))
			Ylva = Cast<AYlva>(OwningPawn);
	}

	GenericsMachineIndex = GetStateMachineIndex("Generics");
}

void UYlvaAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!OwningPawn || !PawnMovementComponent)
		return;

	// Set whether falling or not
	bIsFalling = PawnMovementComponent->IsFalling();

	// Update movement speed for use in the Idle/Run blendspace
	MovementSpeed = Ylva->GetVelocity().Size();
}
