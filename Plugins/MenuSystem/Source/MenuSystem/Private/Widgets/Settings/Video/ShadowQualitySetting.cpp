// Copyright Ali El Saleh 2019

#include "ShadowQualitySetting.h"

void UShadowQualitySetting::Apply()
{
	GameUserSettings->SetShadowQuality(QualityIndex);
}
