// Copyright Ali El Saleh 2019

#include "EffectsQualitySetting.h"

void UEffectsQualitySetting::Apply()
{
	GameUserSettings->SetVisualEffectQuality(QualityIndex);
}
