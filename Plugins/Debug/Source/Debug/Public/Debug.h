// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSuccess, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogNumber, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogBool, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogVector, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogMessage, Display, All)
DECLARE_LOG_CATEGORY_EXTERN(LogObjectValidity, Display, All)

class FDebugModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
