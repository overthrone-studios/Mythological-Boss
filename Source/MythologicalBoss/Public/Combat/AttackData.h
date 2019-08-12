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
	LightAttack_1,
	LightAttack_2,
	LightAttack_3,
	HeavyAttack_1,
	HeavyAttack_2,
	HeavyAttack_3,
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
	UPROPERTY(EditInstanceOnly)
		TEnumAsByte<EAttackType_Combo> Attack;

	UPROPERTY(EditInstanceOnly)
		uint8 bCanDashWithAttack : 1;

	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bCanDashWithAttack"))
		TEnumAsByte<EDashType_Combo> DashType;

	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bCanDashWithAttack"))
		float OffsetDistance = 0.0f;
};
