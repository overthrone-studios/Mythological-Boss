// Copyright Overthrone Studios 2019

#include "Combat/ComboData.h"


void UComboData::Init()
{
	AttackIndex = 0;
	CurrentAttack = Attacks[0];
}

void UComboData::NextAttack()
{
	//if (bDelayBetweenAttacks && !GetWorld()->GetTimerManager().IsTimerActive(DelayTimerHandle))
	//{
	//	const float Min = FMath::Clamp(Delay - RandomDeviation, 0.0f, 100.0f);
	//	const float Max = FMath::Clamp(Delay + RandomDeviation, 0.0f, 100.0f + RandomDeviation);
	//	const float NewDelay = FMath::RandRange(Min, Max);
	//
	//	if (NewDelay > 0.0f)
	//		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UComboData::NextAttack_Internal, NewDelay);
	//	else
	//		NextAttack_Internal();
	//
	//	return;
	//}

	NextAttack_Internal();
}

bool UComboData::IsAtLastAttack()
{
	return AttackIndex == Attacks.Num();
}

bool UComboData::IsDelayEnabled()
{
	return bDelayBetweenAttacks;
}

float UComboData::GetDelayTime()
{
	return Delay;
}

float UComboData::GetDeviation()
{
	return RandomDeviation;
}

FTimerHandle& UComboData::GetDelayTimer()
{
	return DelayTimerHandle;
}

void UComboData::NextAttack_Internal()
{
	AttackIndex++;

	if (AttackIndex >= Attacks.Num())
		AttackIndex = Attacks.Num();

	if (AttackIndex < Attacks.Num())
	{
		CurrentAttack = Attacks[AttackIndex];
	}
}
