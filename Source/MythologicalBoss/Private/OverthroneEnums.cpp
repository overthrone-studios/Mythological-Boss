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
	case Blockable:
		return FString("Blockable");

	case Parryable:
		return FString("Parryable");

	case NoCounter:
		return FString("NoCounter");

	default:
		return FString("None");
	}
}
