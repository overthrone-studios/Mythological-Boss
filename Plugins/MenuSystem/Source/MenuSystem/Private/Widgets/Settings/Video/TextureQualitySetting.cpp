// Copyright Ali El Saleh 2019

#include "TextureQualitySetting.h"

void UTextureQualitySetting::Apply()
{
	GameUserSettings->SetTextureQuality(QualityIndex);
}
