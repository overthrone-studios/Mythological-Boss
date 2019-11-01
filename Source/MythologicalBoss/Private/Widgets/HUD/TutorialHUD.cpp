// Copyright Overthrone Studios 2019

#include "TutorialHUD.h"
#include "OutputDeviceNull.h"

void UTutorialHUD::FadeIn()
{
	if (!IsAnimationPlaying(FadeAnim))
		PlayAnimation(FadeAnim);

	AnimPlayMode = EUMGSequencePlayMode::Forward;

	bIsReady = false;
}

void UTutorialHUD::FadeOut()
{
	if (!IsAnimationPlaying(FadeAnim))
		PlayAnimation(FadeAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);

	AnimPlayMode = EUMGSequencePlayMode::Reverse;

	bIsReady = false;
}

void UTutorialHUD::ProgressTutorial()
{
	FOutputDeviceNull OutputDevice;

	CallFunctionByNameWithArguments(TEXT("ProgressTutorial"), OutputDevice, nullptr, true);
}

void UTutorialHUD::EndTutorial()
{
	FOutputDeviceNull OutputDevice;

	CallFunctionByNameWithArguments(TEXT("EndTutorial"), OutputDevice, nullptr, true);
}
