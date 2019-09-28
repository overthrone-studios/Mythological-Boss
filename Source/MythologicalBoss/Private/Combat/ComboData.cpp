// Copyright Overthrone Studios 2019

#include "Combat/ComboData.h"

void UComboData::Init()
{
	ActionIndex = 0;
	CurrentAction = Actions[0];
}

void UComboData::NextAction()
{
	ActionIndex++;

	if (ActionIndex >= Actions.Num())
		ActionIndex = Actions.Num();

	if (ActionIndex < Actions.Num())
	{
		CurrentAction = Actions[ActionIndex];
	}
}

bool UComboData::IsAtLastAction()
{
	return ActionIndex == Actions.Num();
}

bool UComboData::IsDelayEnabled()
{
	return bDelayBetweenActions;
}

float UComboData::GetActionDelayTime()
{
	return Delay;
}

float UComboData::GetDeviation()
{
	return RandomDeviation;
}

FTimerHandle& UComboData::GetActionDelayTimer()
{
	return TH_ActionDelay;
}
