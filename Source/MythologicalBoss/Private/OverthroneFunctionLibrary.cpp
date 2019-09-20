// Copyright Overthrone Studios 2019

#include "OverthroneFunctionLibrary.h"
#include "Public/OverthroneGameInstance.h"
#include "Public/OverthroneCharacter.h"
#include "Public/OverthroneHUD.h"
#include "Public/BossBattleGameMode.h"
#include "ConfigCacheIni.h"
#include "Mordath.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "SlateApplication.h"

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

bool UOverthroneFunctionLibrary::IsGamepadConnected()
{
	return FSlateApplication::Get().GetPlatformApplication().Get()->IsGamepadAttached();
}
