// Copyright Ali El Saleh 2019

#include "ViewDistanceSetting.h"

void UViewDistanceSetting::Apply()
{
	GameUserSettings->SetViewDistanceQuality(QualityIndex);
}
