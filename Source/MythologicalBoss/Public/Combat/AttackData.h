// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OverthroneEnums.h"
#include "AttackData.generated.h"


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

/**
 * An asset that holds attack data for the boss
 */
UCLASS()
class MYTHOLOGICALBOSS_API UAttackData final : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FString GetCounterTypeAsString() const;
	FString GetCurrentAttackAsString() const;

	// The attack to execute
	UPROPERTY(EditInstanceOnly)
		TEnumAsByte<EAttackType_Mordath> AttackType;

	// The attack animation montage to play
	UPROPERTY(EditInstanceOnly)
		class UAnimMontage* AttackMontage;

	// What type of counter can this attack be countered by the player?
	UPROPERTY(EditInstanceOnly)
		TEnumAsByte<EAttackCounters_Mordath> CounterType;
	
	UPROPERTY(EditInstanceOnly)
		FMordathMontageSectionData Anticipation;

	UPROPERTY(EditInstanceOnly)
		FMordathMontageSectionData Contact;

	UPROPERTY(EditInstanceOnly)
		FMordathMontageSectionData Recovery;

	// Should we snap to the player's location at the pinnacle section of the anim montage?
	UPROPERTY(EditInstanceOnly)
		uint8 bSnapToPlayerLocation : 1;

	// The distance from the player to snap to
	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bSnapToPlayerLocation", ClampMin = 0.0f))
		float DistanceFromPlayer = 200.0f;

	// Should we smoothly lerp to the snap location?
	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bSnapToPlayerLocation"))
		uint8 bLerp : 1;

	// How fast we smoothly snap to the player's location
	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bLerp", ClampMin = 0.0f))
		float LerpSpeed = 10.0f;
};
