// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

UENUM()
enum EBuildConfig
{
	DebugBuild,
	DevelopmentBuild,
	ShippingBuild
};

UENUM()
enum EDashType_Mordath
{
	Dash_Forward,
	Dash_Backward,
	Dash_Left,
	Dash_Right
};

UENUM(BlueprintType)
enum EBossStage_Mordath
{
	Stage_1,
	Stage_2,
	Stage_3
};

UENUM()
enum EAttackType_Player
{
	ATP_Light,
	ATP_Heavy,
	ATP_Special,
	ATP_None UMETA(DisplayName="None")
};

UENUM()
enum EActionType_Mordath
{
	ATM_ShortAttack_1	UMETA(DisplayName="Short Attack 1"),
	ATM_ShortAttack_2	UMETA(DisplayName="Short Attack 2"),
	ATM_ShortAttack_3	UMETA(DisplayName="Short Attack 3"),
	ATM_LongAttack_1	UMETA(DisplayName="Long Attack 1"),
	ATM_LongAttack_2	UMETA(DisplayName="Long Attack 2"),
	ATM_LongAttack_3	UMETA(DisplayName="Long Attack 3"),
	ATM_SpecialAttack_1 UMETA(DisplayName="Special Attack 1"),
	ATM_SpecialAttack_2 UMETA(DisplayName="Special Attack 2"),
	ATM_SpecialAttack_3 UMETA(DisplayName="Special Attack 3"),
	ATM_Kick			UMETA(DisplayName="Kick"),
	ATM_BackHand		UMETA(DisplayName="Back Hand"),
	ATM_Strafe_Left		UMETA(DisplayName="Strafe Left"),
	ATM_Strafe_Right	UMETA(DisplayName="Strafe Right"),
	ATM_Dash_Forward	UMETA(DisplayName="Dash Forward"),
	ATM_Dash_Backward	UMETA(DisplayName="Dash Backward"),
	ATM_Dash_Left		UMETA(DisplayName="Dash Left"),
	ATM_Dash_Right		UMETA(DisplayName="Dash Right"),
	ATM_Teleport		UMETA(DisplayName="Teleport"),
	ATM_Tired			UMETA(DisplayName="Tired"),
	ATM_None			UMETA(DisplayName="None")
};

UENUM()
enum EAttackCounters_Mordath
{
	Parryable,
	Blockable,
	ParryableBlockable,
	NoCounter,
	ACM_None UMETA(DisplayName="None")
};

UENUM()
enum EBossRange_Mordath
{
	SuperClose,
	Close,
	Mid,
	Far
};

/**
 * A header file containing all enums that Overthrone uses
 */
class MYTHOLOGICALBOSS_API UOverthroneEnums
{
public:
	UOverthroneEnums() = default;
	~UOverthroneEnums() = default;

	static FString MordathAttackTypeToString(const EActionType_Mordath& Enum);
	static FString MordathAttackCounterTypeToString(const EAttackCounters_Mordath& Enum);
};
