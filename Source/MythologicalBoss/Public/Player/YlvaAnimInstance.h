// Copyright Overthrone Studios 2019

#pragma once

#include "Animation/AnimInstance.h"
#include "YlvaAnimInstance.generated.h"

/**
 * The animation instance the player will use
 */
UCLASS()
class MYTHOLOGICALBOSS_API UYlvaAnimInstance final : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UYlvaAnimInstance();

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float MovementSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool bIsWalking;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool bIsRunning;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool bIsJumping;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		bool bAcceptLightAttack;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		bool bAcceptSecondLightAttack;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		bool bAcceptThirdLightAttack;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		bool bAcceptHeavyAttack;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		bool bAcceptSecondHeavyAttack;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		bool bAcceptThirdHeavyAttack;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		bool bIsBlocking;

	int32 GenericsMachineIndex;

protected:
	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = "Ylva Anim Instance")
		APawn* OwningPawn{};

	UPROPERTY(BlueprintReadOnly, Category = "Ylva Anim Instance")
		UPawnMovementComponent* PawnMovementComponent{};

	class AYlva* Ylva{};
};
