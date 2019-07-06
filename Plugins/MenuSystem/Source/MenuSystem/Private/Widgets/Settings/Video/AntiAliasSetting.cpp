// Copyright Ali El Saleh 2019

#include "AntiAliasSetting.h"

void UAntiAliasSetting::Apply()
{
	GameUserSettings->SetAntiAliasingQuality(QualityIndex);
}
