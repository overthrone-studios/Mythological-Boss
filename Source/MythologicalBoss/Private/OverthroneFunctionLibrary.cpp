// Copyright Overthrone Studios 2019

#include "OverthroneFunctionLibrary.h"
#include "Public/OverthroneGameInstance.h"
#include "Public/OverthroneHUD.h"
#include "Public/BossBattleGameMode.h"
#include "ConfigCacheIni.h"
#include "Mordath.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

FString UOverthroneFunctionLibrary::GetProjectVersion()
{
	FString ProjectVersion;

	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), ProjectVersion, GGameIni);

	return ProjectVersion;
}

ACharacter* UOverthroneFunctionLibrary::GetBossCharacter(UWorld* InWorld)
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
