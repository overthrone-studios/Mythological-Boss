// Copyright Overthrone Studios 2019

#pragma once

#include "OverthroneAnimInstance.h"
#include "YlvaAnimInstance.generated.h"

/**
 * The animation instance the player will use
 */
UCLASS()
class MYTHOLOGICALBOSS_API UYlvaAnimInstance final : public UOverthroneAnimInstance
{
	GENERATED_BODY()
	
public:
	void LeaveAllStates() override;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bCanDashForward : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bCanDashBackward : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bCanDashLeft : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bCanDashRight : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bIsShieldHit : 1;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bIsBlocking : 1;
};
