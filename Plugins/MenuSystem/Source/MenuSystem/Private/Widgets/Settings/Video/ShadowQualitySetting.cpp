// Copyright Ali El Saleh 2019

#include "ShadowQualitySetting.h"

void UShadowQualitySetting::Apply()
{
	Super::Apply();

	GameUserSettings->SetShadowQuality(QualityIndex);
}
