// Copyright Overthrone Studios 2019

#include "OverthroneFunctionLibrary.h"

#include "Public/OverthroneGameInstance.h"
#include "Public/OverthroneCharacter.h"
#include "Public/OverthroneGameState.h"
#include "Public/OverthroneHUD.h"
#include "Public/BossBattleGameMode.h"

#include "ConfigCacheIni.h"
#include "EngineUtils.h"
#include "SlateApplication.h"

#include "Kismet/GameplayStatics.h"

#include "Log.h"

FString UOverthroneFunctionLibrary::GetProjectVersion()
{
	FString ProjectVersion;

	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), ProjectVersion, GGameIni);

	return ProjectVersion;
}

FString UOverthroneFunctionLibrary::GetBuildConfigurationAsString()
{
	FString BuildConfig;
	
	GConfig->GetString(TEXT("/Script/UnrealEd.ProjectPackagingSettings"), TEXT("BuildConfiguration"), BuildConfig, GGameIni);
	BuildConfig.RemoveAt(0, 5);

	return BuildConfig;
}

TEnumAsByte<EBuildConfig> UOverthroneFunctionLibrary::GetBuildConfiguration()
{
#if UE_BUILD_SHIPPING
		return ShippingBuild;
#elif UE_BUILD_DEVELOPMENT
		return DevelopmentBuild;
#elif UE_BUILD_DEBUG
		return DebugBuild;
#endif
}

AOverthroneCharacter* UOverthroneFunctionLibrary::GetPlayerCharacter(UObject* WorldContextObject)
{
	return Cast<AOverthroneCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));
}

AOverthroneCharacter* UOverthroneFunctionLibrary::GetBossCharacter(UObject* Object, const FName& Tag)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(Object, Tag, FoundActors);

	for (auto Actor : FoundActors)
	{
		return Cast<AOverthroneCharacter>(Actor);
	}

	return nullptr;
}

UOverthroneGameInstance* UOverthroneFunctionLibrary::GetGameInstance(const UObject* WorldContext)
{
	return Cast<UOverthroneGameInstance>(UGameplayStatics::GetGameInstance(WorldContext));
}

AOverthroneGameState* UOverthroneFunctionLibrary::GetGameState(const UObject* WorldContext)
{
	return Cast<AOverthroneGameState>(UGameplayStatics::GetGameState(WorldContext));
}

AOverthroneHUD* UOverthroneFunctionLibrary::GetHUD(APlayerController* InPlayerController)
{
	return Cast<AOverthroneHUD>(InPlayerController->GetHUD());
}

UMasterHUD* UOverthroneFunctionLibrary::GetMasterHUD(APlayerController* InPlayerController)
{
	return GetHUD(InPlayerController)->GetMasterHUD();
}

ABossBattleGameMode* UOverthroneFunctionLibrary::GetGameMode(const UObject* WorldContext)
{
	return Cast<ABossBattleGameMode>(UGameplayStatics::GetGameMode(WorldContext));
}

void UOverthroneFunctionLibrary::PauseGame(const UObject* WorldContext, APlayerController* InPlayerController)
{
	UGameplayStatics::SetGamePaused(WorldContext, true);

	if (InPlayerController)
	{
		InPlayerController->bShowMouseCursor = true;
		InPlayerController->SetInputMode(FInputModeUIOnly());
	}
}

void UOverthroneFunctionLibrary::UnPauseGame(const UObject* WorldContext, APlayerController* InPlayerController)
{
	UGameplayStatics::SetGamePaused(WorldContext, false);

	if (InPlayerController)
	{
		InPlayerController->bShowMouseCursor = false;
		InPlayerController->SetInputMode(FInputModeGameOnly());
	}
}

bool UOverthroneFunctionLibrary::IsGamepadConnected()
{
	return FSlateApplication::Get().GetPlatformApplication().Get()->IsGamepadAttached();
}

FString UOverthroneFunctionLibrary::ConvertKeyToString(const FKey Key)
{
	if (Key == EKeys::Gamepad_FaceButton_Bottom)
		return FString("A");

	if (Key == EKeys::Gamepad_FaceButton_Right)
		return FString("B");

	if (Key == EKeys::Gamepad_FaceButton_Left)
		return FString("X");

	if (Key == EKeys::Gamepad_FaceButton_Top)
		return FString("Y");

	if (Key == EKeys::Gamepad_RightShoulder)
		return FString("RB");

	if (Key == EKeys::Gamepad_LeftShoulder)
		return FString("LB");

	if (Key == EKeys::Gamepad_RightTrigger)
		return FString("RT");

	if (Key == EKeys::Gamepad_LeftTrigger)
		return FString("LT");

	if (Key == EKeys::Gamepad_LeftThumbstick)
		return FString("LeftStick");

	if (Key == EKeys::Gamepad_RightThumbstick)
		return FString("RightStick");

	if (Key == EKeys::Gamepad_DPad_Down)
		return FString("DPadDown");

	if (Key == EKeys::Gamepad_DPad_Up)
		return FString("DPadUp");

	if (Key == EKeys::Gamepad_DPad_Right)
		return FString("DPadRight");

	if (Key == EKeys::Gamepad_DPad_Left)
		return FString("DPadLeft");

	if (Key == EKeys::LeftMouseButton)
		return FString("LMB");

	if (Key == EKeys::RightMouseButton)
		return FString("RMB");

	if (Key == EKeys::MiddleMouseButton)
		return FString("MMB");

	if (Key == EKeys::NumPadOne)
		return FString("One");

	if (Key == EKeys::NumPadTwo)
		return FString("Two");

	if (Key == EKeys::NumPadThree)
		return FString("Three");

	if (Key == EKeys::NumPadFour)
		return FString("Four");

	if (Key == EKeys::NumPadFive)
		return FString("Five");

	if (Key == EKeys::NumPadSix)
		return FString("Six");

	if (Key == EKeys::NumPadSeven)
		return FString("Seven");

	if (Key == EKeys::NumPadEight)
		return FString("Eight");

	if (Key == EKeys::NumPadNine)
		return FString("Nine");

	if (Key == EKeys::NumPadZero)
		return FString("Zero");

	if (Key == EKeys::Up)
		return FString("UpArrow");
	
	if (Key == EKeys::Down)
		return FString("DownArrow");

	if (Key == EKeys::Left)
		return FString("LeftArrow");

	if (Key == EKeys::Right)
		return FString("RightArrow");

	return Key.ToString();
}

FVector UOverthroneFunctionLibrary::SmoothStop(const FVector& A, const FVector& B, const float Alpha, const float Exp)
{
	float NewExp = 1.0f;

	for (float i = 0.0f; i < Exp; ++i)
		NewExp *= 1.0f - Alpha;

	const float ModifiedAlpha = 1.0f - NewExp;

	return FMath::Lerp(A, B, ModifiedAlpha);
}

void UOverthroneFunctionLibrary::SetupTimeline(UObject* Object, FTimeline& InTimeline, UCurveFloat* InCurveFloat, const bool bLooping, const float InPlaybackSpeed, const FName& TimelineCallbackFuncName, const FName& TimelineFinishedCallbackFuncName)
{
	// Timeline Initialization
	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(Object, TimelineCallbackFuncName);

	if (InCurveFloat)
	{
		InTimeline.AddInterpFloat(InCurveFloat, TimelineCallback);
		InTimeline.SetTimelineLength(InCurveFloat->FloatCurve.Keys[InCurveFloat->FloatCurve.Keys.Num() - 1].Time);
	}
	else
	{
		ULog::DebugMessage(ERROR, FString("Failed to initialize timeline. A curve float asset is missing!"), true);
	}

	if (TimelineCallbackFuncName != NAME_None)
	{
		FOnTimelineEvent TimelineFinishedCallback;
		TimelineFinishedCallback.BindUFunction(Object, TimelineFinishedCallbackFuncName);
		InTimeline.SetLooping(bLooping);
		InTimeline.SetPlayRate(InPlaybackSpeed);
		InTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
		InTimeline.SetTimelineLengthMode(TL_TimelineLength);
	}
}
