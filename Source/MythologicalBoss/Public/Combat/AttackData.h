// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackData.generated.h"

UENUM()
enum EDashType_Combo
{
	Dash_Forward,
	Dash_Backward,
	Dash_Left,
	Dash_Right
};

UENUM()
enum EAttackType_Combo
{
	ShortAttack_1,
	ShortAttack_2,
	ShortAttack_3,
	LongAttack_1,
	LongAttack_2,
	LongAttack_3,
	SpecialAttack_1,
	SpecialAttack_2,
	SpecialAttack_3
};

/**
 * An asset that holds attack data for the boss
 */
UCLASS()
class MYTHOLOGICALBOSS_API UAttackData final : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// The attack to execute
	UPROPERTY(EditInstanceOnly)
		TEnumAsByte<EAttackType_Combo> Attack;

	UPROPERTY(EditInstanceOnly)
		class UAnimMontage* AttackMontage;
	
	// How fast do we rotate while in the anticipation section of the anim montage above? (0.0 = Instant, 10.0+ = Fast)
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float AnticipationRotationSpeed = 10.0f;

	// How fast do we rotate while in the contact section of the anim montage above? (0.0 = Instant, 10.0+ = Fast)
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float ContactRotationSpeed = 5.0f;

	// How fast do we rotate while in the recovery section of the anim montage above? (0.0 = Instant, 10.0+ = Fast)
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float RecoveryRotationSpeed = 3.0f;

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

	// Can we dash before we attack?
	UPROPERTY(EditInstanceOnly)
		uint8 bCanDashWithAttack : 1;

	// Can we teleport before we attack?
	UPROPERTY(EditInstanceOnly)
		uint8 bCanTeleportWithAttack : 1;

	// The type of dash to use before attacking
	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bCanDashWithAttack"))
		TEnumAsByte<EDashType_Combo> DashType;

	// The distance offset. (Does not affect Left or Right dash)
	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bCanDashWithAttack", ClampMin = 0.0f, ClampMax = 1000000.0f))
		float AcceptanceRadius = 0.0f;

	// The speed of the dash
	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bCanDashWithAttack", ClampMin = 0.01f, ClampMax = 1000.0f))
		float Speed = 1.0f;
};
