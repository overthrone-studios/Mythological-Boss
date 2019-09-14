// Copyright Overthrone Studios 2019

#include "TutorialHUD.h"
#include "OutputDeviceNull.h"

void UTutorialHUD::SlideIn()
{
	PlayAnimation(SlideAnim);
}

void UTutorialHUD::SlideOut()
{
	PlayAnimation(SlideAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
}

void UTutorialHUD::ProgressTutorial()
{
	FOutputDeviceNull OutputDevice;

	CallFunctionByNameWithArguments(TEXT("ProgressTutorial"), OutputDevice, nullptr, true);
}
