// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MordathAnimInstance.generated.h"

/**
 * The animation instance the boss character will use
 */
UCLASS()
class MYTHOLOGICALBOSS_API UMordathAnimInstance final : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float MovementSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool bIsWalking;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool bIsRunning;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		bool bIsHit;

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

	int32 GenericsMachineIndex;

	int32 IdleLoopCount = 0;
	
protected:
	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = "Mordath Anim Instance")
		APawn* OwningPawn{};

	UPROPERTY(BlueprintReadOnly, Category = "Mordath Anim Instance")
		UPawnMovementComponent* PawnMovementComponent{};

	class AMordath* Mordath{};
};
