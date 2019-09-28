// Copyright Overthrone Studios 2019

#include "OverthroneEnums.h"

FString UOverthroneEnums::MordathAttackTypeToString(const EActionType_Mordath& Enum)
{
	switch (Enum)
	{
	case ShortAttack_1:
		return FString("Short Attack 1");

	case ShortAttack_2:
		return FString("Short Attack 2");

	case ShortAttack_3:
		return FString("Short Attack 3");

	case LongAttack_1:
		return FString("Long Attack 1");

	case LongAttack_2:
		return FString("Long Attack 2");

	case LongAttack_3:
		return FString("Long Attack 3");

	case SpecialAttack_1:
		return FString("Special Attack 1");

	case SpecialAttack_2:
		return FString("Special Attack 2");

	case SpecialAttack_3:
		return FString("Special Attack 3");

	case Kick:
		return FString("Kick");
		
	case BackHand:
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
