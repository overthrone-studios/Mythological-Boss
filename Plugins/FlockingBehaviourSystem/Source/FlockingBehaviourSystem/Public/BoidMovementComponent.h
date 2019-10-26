// Copyright Ali El Saleh, 2019

#pragma once

#include "GameFramework/FloatingPawnMovement.h"
#include "BoidMovementComponent.generated.h"

/**
 * Controls the movement of a boid
 */
UCLASS()
class FLOCKINGBEHAVIOURSYSTEM_API UBoidMovementComponent : public UFloatingPawnMovement
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class ABoid* Owner;
};
