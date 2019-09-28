// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OverthroneEnums.h"
#include "MordathActionData.generated.h"


USTRUCT(BlueprintType)
struct FMordathMontageSectionData
{
	GENERATED_BODY()

	// How fast do we rotate while in the anticipation section of the anim montage above? (0.0 = Instant, 10.0+ = Fast)
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float RotationSpeed = 10.0f;

	// The rate/speed of the animation (0.0 = No rate, 1.0 = Normal, 2.0+ = Fast)
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float PlayRate = 1.0f;
};

USTRUCT(BlueprintType)
struct FMordathMontagePinnacleData
{
	GENERATED_BODY()

	// Should we snap to the player's location at the pinnacle section of the anim montage?
	UPROPERTY(EditInstanceOnly)
		uint8 bSnapToPlayerLocation : 1;

	// Should we snap our rotation to face the player?
	UPROPERTY(EditInstanceOnly)
		uint8 bSnapRotation : 1;

	// The distance from the player to snap to
	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bSnapToPlayerLocation", ClampMin = 0.0f))
		float DistanceFromPlayer = 200.0f;

	// Should we smoothly lerp to the snap location?
	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bSnapToPlayerLocation"))
		uint8 bSmooth : 1;

	// How fast we smoothly snap to the player's location
	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bSmooth", ClampMin = 0.0f))
		float Speed = 10.0f;
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
		TEnumAsByte<EAttackCounters_Mordath> CounterType;
	
	// The data to use while in the Anticipation section of the animation
	UPROPERTY(EditInstanceOnly)
		FMordathMontageSectionData Anticipation;

	// The data to use while in the Pinnacle section of the animation
	UPROPERTY(EditInstanceOnly)
		FMordathMontagePinnacleData Pinnacle;

	// The data to use while in the Contact section of the animation
	UPROPERTY(EditInstanceOnly)
		FMordathMontageSectionData Contact;

	// The data to use while in the Recovery section of the animation
	UPROPERTY(EditInstanceOnly)
		FMordathMontageSectionData Recovery;
};
