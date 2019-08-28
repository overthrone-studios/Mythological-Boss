// Copyright Overthrone Studios 2019

#pragma once

#include "Animation/AnimInstance.h"
#include "OverthroneAnimInstance.generated.h"

/**
 * Base class for an overthrone character's anim instance
 */
UCLASS()
class MYTHOLOGICALBOSS_API UOverthroneAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Overthrone Anim Instance")
		virtual void LeaveAllStates();

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float BlendAlpha = 1.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float AnimTimeRemaining = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float MovementSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float MovementDirection = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bIsHit : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bIsDead : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bAcceptLightAttack : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bAcceptSecondLightAttack : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bAcceptThirdLightAttack : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bAcceptHeavyAttack : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bAcceptSecondHeavyAttack : 1;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bAcceptThirdHeavyAttack : 1;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Combat")
		uint8 bLogDirection : 1;

	// Used to get a reference to the active anim state machine, and to query time remaining
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		int32 ActiveStateMachine = 0;

protected:
	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Category = "Overthrone Anim Instance", meta = (BlueprintThreadSafe))
		float GetAnimTimeRemaining();

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Anim Instance")
		APawn* OwningPawn{};

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Anim Instance")
		UPawnMovementComponent* PawnMovementComponent{};
};
