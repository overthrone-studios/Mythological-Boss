// Copyright Overthrone Studios 2019

#include "OverthroneFunctionLibrary.h"
#include "Public/OverthroneGameInstance.h"
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
	ACharacter* BossCharacter = nullptr;

	for (const TActorIterator<AMordath> ActorItr(InWorld); ActorItr;)
	{
		BossCharacter = *ActorItr;
		break;
	}

	return BossCharacter;
}

UOverthroneGameInstance* UOverthroneFunctionLibrary::GetGameInstance(const UObject* WorldContext)
{
	return Cast<UOverthroneGameInstance>(UGameplayStatics::GetGameInstance(WorldContext));
}
