// Copyright Ali El Saleh, 2019

#pragma once

#include "GameFramework/Controller.h"
#include "BoidController.generated.h"

/**
 * The controller that will possess the boid
 */
UCLASS()
class FLOCKINGBEHAVIOURSYSTEM_API ABoidController : public AController
{
	GENERATED_BODY()

public:
	ABoidController();
};
