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

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float ForwardInput = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float RightInput = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bIsShieldHit : 1;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bIsBlocking : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bIsMoving : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bCanCharge : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bChargeReleased : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bCanParry : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		float LeanRollAmount = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		float LeanPitchAmount = 0.0f;
};
