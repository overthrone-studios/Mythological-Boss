// Copyright Overthrone Studios 2019

#include "OverthroneEnums.h"

FString UOverthroneEnums::MordathAttackTypeToString(const EActionType_Mordath& Enum)
{
	switch (Enum)
	{
	case ATM_ShortAttack_1:
		return FString("Short Attack 1");

	case ATM_ShortAttack_2:
		return FString("Short Attack 2");

	case ATM_ShortAttack_3:
		return FString("Short Attack 3");

	case ATM_LongAttack_1:
		return FString("Long Attack 1");

	case ATM_LongAttack_2:
		return FString("Long Attack 2");

	case ATM_LongAttack_3:
		return FString("Long Attack 3");

	case ATM_SpecialAttack_1:
		return FString("Special Attack 1");

	case ATM_SpecialAttack_2:
		return FString("Special Attack 2");

	case ATM_SpecialAttack_3:
		return FString("Special Attack 3");

	case ATM_Kick:
		return FString("Kick");
		
	case ATM_BackHand:
		return FString("BackHand");

	default:
		return FString("None");
	}
}

FString UOverthroneEnums::MordathAttackCounterTypeToString(const EAttackCounters_Mordath& Enum)
{
	switch (Enum)
	{
	case ACM_Blockable:
		return FString("Blockable");

	case ACM_Parryable:
		return FString("Parryable");

	case ACM_NoCounter:
		return FString("NoCounter");

	default:
		return FString("None");
	}
}

FString UOverthroneEnums::PlayerAttackTypeToString(const EAttackType_Player& Enum)
{
	switch (Enum)
	{
	case ATP_Light:
		return FString("Light");

	case ATP_Heavy:
		return FString("Heavy");

	case ATP_Special:
		return FString("Special");

	case ATP_Charge:
		return FString("Charge");
		
	case ATP_Dash:
		return FString("Dash");

	default:
		return FString("None");
	}
}

FString UOverthroneEnums::DifficultyOptionToString(const EDifficultyOptions& Enum)
{
	switch (Enum)
	{
	case DO_Casual:
		return FString("Casual");

	case DO_Experienced:
		return FString("Experienced");

	case DO_Realistic:
		return FString("Realistic");

	default:
		return FString("None");
	}
}
