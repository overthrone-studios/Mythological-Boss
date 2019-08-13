// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "OverthroneAnimInstance.h"
#include "MordathAnimInstance.generated.h"

/**
 * The animation instance the boss character will use
 */
UCLASS()
class MYTHOLOGICALBOSS_API UMordathAnimInstance final : public UOverthroneAnimInstance
{
	GENERATED_BODY()

public:
	void LeaveAllStates() override;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bIsStunned : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bIsBeaten : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Misc")
		uint8 bCanLaugh : 1;
};
