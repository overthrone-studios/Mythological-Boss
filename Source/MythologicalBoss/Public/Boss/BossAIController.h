// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

/**
 * The controller that controls the boss in the game
 */
UCLASS()
class MYTHOLOGICALBOSS_API ABossAIController final : public AAIController
{
	GENERATED_BODY()
	
public:
	ABossAIController();

	void Possess(APawn* InPawn) override;

	void SetPlayerLocation(const FVector& Location);

protected:
	// Cache game instance
	class UOverthroneGameInstance* GameInstance;

	UPROPERTY(transient) // transient disables serialization
		UBlackboardComponent* BlackBoardComponent;

	UPROPERTY(transient) // transient disables serialization
		class UBehaviorTreeComponent* BehaviourTreeComponent;

	int32 PlayerLocationKeyID;
};
