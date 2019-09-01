// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "OverthroneAnimInstance.h"
#include "MordathAnimInstance.generated.h"

UENUM(BlueprintType)
enum EBossStage
{
	Stage_1,
	Stage_2,
	Stage_3
};

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

	UPROPERTY(BlueprintReadOnly, Category = "Misc")
		uint8 bCanTeleport : 1;

	// Used to get a reference to the anim state machines, and to query time remaining
	int32 StateMachines[3];

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		TEnumAsByte<EBossStage> CurrentStage = Stage_1;

protected:
	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
