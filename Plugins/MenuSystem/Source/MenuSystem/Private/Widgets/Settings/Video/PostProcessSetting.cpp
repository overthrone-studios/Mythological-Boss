// Copyright Ali El Saleh 2019

#include "PostProcessSetting.h"

void UPostProcessSetting::Apply()
{
	Super::Apply();

	GameUserSettings->SetPostProcessingQuality(QualityIndex);
}
