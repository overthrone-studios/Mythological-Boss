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
	
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float AnticipationRotationSpeed = 10.0f;

	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float ContactRotationSpeed = 5.0f;

	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f))
		float RecoveryRotationSpeed = 3.0f;

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
