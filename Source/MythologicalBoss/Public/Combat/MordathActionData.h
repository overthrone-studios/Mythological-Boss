// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OverthroneEnums.h"
#include "AlphaBlend.h"
#include "MordathActionData.generated.h"

USTRUCT(BlueprintType)
struct FMontageActionData_Base
{
	GENERATED_BODY()

	// Should we snap to the player's location at the pinnacle section of the anim montage?
	UPROPERTY(EditInstanceOnly)
		uint8 bSnapToPlayerLocation : 1;

	// Should we smoothly lerp to the snap location?
	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bSnapToPlayerLocation"))
		uint8 bSmoothSnap : 1;

	// The type of blending to use
	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bSnapToPlayerLocation"))
		EAlphaBlendOption BlendOption;

	// How fast we smoothly snap to the player's location
	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bSmoothSnap", ClampMin = 0.0f))
		float Speed = 10.0f;

	// The distance from the player to snap to
	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bSnapToPlayerLocation", ClampMin = 0.0f))
		float DistanceFromPlayer = 200.0f;
};

USTRUCT(BlueprintType)
struct FMontageActionData_Mordath : public FMontageActionData_Base
{
	GENERATED_BODY()

	// How fast do we rotate while in the anticipation section of the anim montage above? (0.0 = No rotation, 1.0 = Slow, 10.0+ = Fast)
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float RotationSpeed = 10.0f;

	// The rate/speed of the animation (0.0 = No rate, 1.0 = Normal, 2.0+ = Fast)
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float PlayRate = 1.0f;
};

USTRUCT(BlueprintType)
struct FMontagePinnacleData_Mordath : public FMontageActionData_Base
{
	GENERATED_BODY()

	// Should we snap our rotation to face the player?
	UPROPERTY(EditInstanceOnly)
		uint8 bSnapRotation : 1;
};

/**
 * An asset that holds action data for Mordath
 */
UCLASS()
class MYTHOLOGICALBOSS_API UMordathActionData final : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FString GetCounterTypeAsString() const;
	FString GetCurrentActionAsString() const;

	// The attack to execute
	UPROPERTY(EditInstanceOnly)
		TEnumAsByte<EActionType_Mordath> ActionType;

	// The attack animation montage to play
	UPROPERTY(EditInstanceOnly)
		class UAnimMontage* ActionMontage;

	// The amount of damage to deal while executing this action
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float ActionDamage = 20.0f;

	// What type of counter can this attack be countered by the player?
	UPROPERTY(EditInstanceOnly)
		TEnumAsByte<EAttackCounters_Mordath> CounterType = ACM_None;
	
	// Should we constantly face the player during this action?
	UPROPERTY(EditInstanceOnly, DisplayName = "Always Face Player?")
		uint8 bConstantlyFacePlayer : 1;

	// The data to use while in the Anticipation section of the animation
	UPROPERTY(EditInstanceOnly, meta = (EditCondition = "!bConstantlyFacePlayer"))
		FMontageActionData_Mordath Anticipation;

	// The data to use while in the Pinnacle section of the animation
	UPROPERTY(EditInstanceOnly, meta = (EditCondition = "!bConstantlyFacePlayer"))
		FMontagePinnacleData_Mordath Pinnacle;

	// The data to use while in the Contact section of the animation
	UPROPERTY(EditInstanceOnly, meta = (EditCondition = "!bConstantlyFacePlayer"))
		FMontageActionData_Mordath Contact;

	// The data to use while in the Recovery section of the animation
	UPROPERTY(EditInstanceOnly, meta = (EditCondition = "!bConstantlyFacePlayer"))
		FMontageActionData_Mordath Recovery;

//private:
//	FTimerHandle TH_ActionExpiry;
};
