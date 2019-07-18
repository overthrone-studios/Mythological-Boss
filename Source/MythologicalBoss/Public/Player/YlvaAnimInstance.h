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

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsWalking;
	
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsRunning;
	
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsFalling;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsJumping;
	
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
		bool bIsAttacking;
	
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
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
