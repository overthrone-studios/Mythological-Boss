// Copyright Overthrone Studios 2019

#include "OverthroneFunctionLibrary.h"
#include "ConfigCacheIni.h"
#include "Mordath.h"
#include "EngineUtils.h"

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
