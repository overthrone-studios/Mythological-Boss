// Copyright Overthrone Studios 2019

#include "OverthroneFunctionLibrary.h"
#include "ConfigCacheIni.h"

FString UOverthroneFunctionLibrary::GetProjectVersion()
{
	FString ProjectVersion;

	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), ProjectVersion, GGameIni);

	return ProjectVersion;
}
