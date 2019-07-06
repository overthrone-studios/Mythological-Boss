// Copyright Ali El Saleh 2019

#include "PostProcessSetting.h"

void UPostProcessSetting::Apply()
{
	GameUserSettings->SetPostProcessingQuality(QualityIndex);
}
