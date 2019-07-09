// Copyright Ali El Saleh 2019

#include "FoliageQualitySetting.h"

void UFoliageQualitySetting::Apply()
{
	Super::Apply();

	GameUserSettings->SetFoliageQuality(QualityIndex);
}
