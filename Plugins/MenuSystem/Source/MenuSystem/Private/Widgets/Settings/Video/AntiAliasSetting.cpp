// Copyright Ali El Saleh 2019

#include "AntiAliasSetting.h"

void UAntiAliasSetting::Apply()
{
	Super::Apply();

	GameUserSettings->SetAntiAliasingQuality(QualityIndex);
}
