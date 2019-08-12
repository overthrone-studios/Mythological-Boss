// Copyright Overthrone Studios 2019

#include "BossAIController.h"
#include "OverthroneGameInstance.h"
#include "Mordath.h"
#include "Engine/World.h"


ABossAIController::ABossAIController()
{
}

void ABossAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	GameInstance = Cast<UOverthroneGameInstance>(GetWorld()->GetGameInstance());
	Mordath = Cast<AMordath>(InPawn);
}
