// Copyright Overthrone Studios 2019

#include "TutorialData.h"


FText UTutorialData::GetTutorialProgress()
{
	return FText::FromString(FString::FromInt(CurrentCount) + "/" + FString::FromInt(MaxCount));
}

