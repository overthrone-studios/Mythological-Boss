// Copyright Overthrone Studios 2019

#include "FeatData.h"

FText UFeatData::GetFeatProgress()
{
	return FText::FromString(FString::FromInt(CurrentCount) + "/" + FString::FromInt(MaxCount));
}
