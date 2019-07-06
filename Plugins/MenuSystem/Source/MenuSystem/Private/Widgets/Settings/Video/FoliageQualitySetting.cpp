// Copyright Ali El Saleh 2019

#include "FoliageQualitySetting.h"

void UFoliageQualitySetting::Apply()
{
	GameUserSettings->SetFoliageQuality(QualityIndex);
}
