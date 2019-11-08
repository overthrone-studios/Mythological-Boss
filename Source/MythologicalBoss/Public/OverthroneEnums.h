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

UENUM(BlueprintType)
enum EBossStage_Mordath
{
	Stage_1,
	Stage_2,
	Stage_3,
	Stage_None
};

UENUM(BlueprintType)
enum EDifficultyOptions
{
	DO_Casual UMETA(DisplayName = "Casual"),
	DO_Experienced UMETA(DisplayName = "Experienced"),
	DO_Realistic UMETA(DisplayName = "Realistic"),
	DO_None UMETA(DisplayName = "None")
};

UENUM()
enum EAttackType_Player
{
	ATP_Light		UMETA(DisplayName="Light"),
	ATP_Heavy		UMETA(DisplayName="Heavy"),
	ATP_Special		UMETA(DisplayName="Special"),
	ATP_Dash		UMETA(DisplayName="Dash"),
	ATP_None		UMETA(DisplayName="None")
};

UENUM(BlueprintType)
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
	ATM_Walk_Forward	UMETA(DisplayName="Walk Forward"),
	ATM_Walk_Backward	UMETA(DisplayName="Walk Backward"),
	ATM_Strafe_Left		UMETA(DisplayName="Strafe Left"),
	ATM_Strafe_Right	UMETA(DisplayName="Strafe Right"),
	ATM_Dash_Forward	UMETA(DisplayName="Dash Forward"),
	ATM_Dash_Backward	UMETA(DisplayName="Dash Backward"),
	ATM_Dash_Left		UMETA(DisplayName="Dash Left"),
	ATM_Dash_Right		UMETA(DisplayName="Dash Right"),
	ATM_Teleport		UMETA(DisplayName="Teleport"),
	ATM_Tired			UMETA(DisplayName="Tired"),
	ATM_Other			UMETA(DisplayName="Other"),
	ATM_None			UMETA(DisplayName="None")
};

UENUM(BlueprintType)
enum EAttackCounters_Mordath
{
	ACM_Parryable			UMETA(DisplayName="Parryable"),
	ACM_Blockable			UMETA(DisplayName="Blockable"),
	ACM_ParryableBlockable	UMETA(DisplayName="Parryable and Blockable"),
	ACM_NoCounter			UMETA(DisplayName="No Counter"),
	ACM_None				UMETA(DisplayName="None"),
	ACM_Size
};

UENUM()
enum EBossRange_Mordath
{
	BRM_Close				UMETA(DisplayName="Close"),
	BRM_Mid					UMETA(DisplayName="Mid"),
	BRM_Far					UMETA(DisplayName="Far"),
	BRM_SuperClose			UMETA(DisplayName="Super Close"),
	BRM_AnyRange			UMETA(DisplayName="Any Range")
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
	static FString PlayerAttackTypeToString(const EAttackType_Player& Enum);
	static FString DifficultyOptionToString(const EDifficultyOptions& Enum);
};
