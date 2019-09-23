// Copyright Overthrone Studios 2019

#include "MordathStageData.h"

void FThinkStateData::CalculateThinkTime()
{
	const float Min = ThinkTime - RandomDeviation;
	const float Max = ThinkTime + RandomDeviation;

	ThinkTime = FMath::Clamp(FMath::FRandRange(Min, Max), 0.0f, Max);
}

void FRetreatStateData::CalculateRetreatTime()
{
	const float Min = RetreatTime - RandomDeviation;
	const float Max = RetreatTime + RandomDeviation;

	RetreatTime = FMath::Clamp(FMath::FRandRange(Min, Max), 0.0f, Max);
}