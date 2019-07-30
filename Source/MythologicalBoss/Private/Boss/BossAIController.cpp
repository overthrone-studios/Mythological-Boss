// Copyright Overthrone Studios 2019

#include "BossAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "OverthroneGameInstance.h"
#include "Mordath.h"


ABossAIController::ABossAIController()
{
	// Blackboard Component
	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(FName("Blackboard Component"));

	// Behaviour Tree Component
	BehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("Behaviour Tree Component"));

	PlayerLocationKeyID = BlackBoardComponent->GetKeyID(FName("PlayerLocation"));
}

void ABossAIController::StopBT()
{
	BehaviourTreeComponent->StopTree();
}

void ABossAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	GameInstance = Cast<UOverthroneGameInstance>(GetWorld()->GetGameInstance());

	// Initialize all blackboard keys and start
	AMordath* Mordath = Cast<AMordath>(InPawn);
	if (Mordath && Mordath->GetBT())
	{
		BlackBoardComponent->InitializeBlackboard(*Mordath->GetBT()->BlackboardAsset);

		// Set blackboard key values
		BlackBoardComponent->SetValue<UBlackboardKeyType_Object>(FName("SelfActor"), Mordath);

		BehaviourTreeComponent->StartTree(*Mordath->GetBT());
	}
}

void ABossAIController::SetPlayerLocation(const FVector& Location)
{
	if (BlackBoardComponent)
		BlackBoardComponent->SetValue<UBlackboardKeyType_Vector>(FName("PlayerLocation"), Location);
}
