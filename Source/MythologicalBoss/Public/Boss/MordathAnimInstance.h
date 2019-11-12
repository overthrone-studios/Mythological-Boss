// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "OverthroneAnimInstance.h"
#include "OverthroneEnums.h"
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

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bCanKick : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bWantsSideStepDash : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bIsDashingForward : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bIsDashingBackward : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bIsInvincible : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		uint8 bCanRoar : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Misc")
		uint8 bIsRecovering : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Misc")
		uint8 bCanTeleport : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Misc")
		uint8 bIsThinking : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		TEnumAsByte<EBossStage_Mordath> CurrentStage = Stage_1;

protected:
	void NativeInitializeAnimation() override;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		class AMordath* Mordath;
};
