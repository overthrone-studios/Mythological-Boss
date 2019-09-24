// Copyright Overthrone Studios 2019

#include "OverthroneFunctionLibrary.h"

#include "Public/OverthroneGameInstance.h"
#include "Public/OverthroneGameState.h"
#include "Public/OverthroneCharacter.h"
#include "Public/OverthroneHUD.h"
#include "Public/BossBattleGameMode.h"

#include "Mordath.h"

#include "ConfigCacheIni.h"
#include "EngineUtils.h"
#include "SlateApplication.h"

#include "Kismet/GameplayStatics.h"

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

AOverthroneCharacter* UOverthroneFunctionLibrary::GetBossCharacter(UWorld* InWorld)
{
	for (const TActorIterator<AMordath> ActorItr(InWorld); ActorItr;)
	{
		return *ActorItr;
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
	InPlayerController->bShowMouseCursor = true;
	InPlayerController->SetInputMode(FInputModeUIOnly());
}

void UOverthroneFunctionLibrary::UnPauseGame(const UObject* WorldContext, APlayerController* InPlayerController)
{
	UGameplayStatics::SetGamePaused(WorldContext, false);
	InPlayerController->bShowMouseCursor = false;
	InPlayerController->SetInputMode(FInputModeGameOnly());
}

bool UOverthroneFunctionLibrary::IsGamepadConnected()
{
	return FSlateApplication::Get().GetPlatformApplication().Get()->IsGamepadAttached();
}
