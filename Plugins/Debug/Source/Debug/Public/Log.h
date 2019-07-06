// Copyright Overthrone Studios 2019

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Log.generated.h"

UENUM()
enum ELogType
{
	INFO,
	SUCCESS,
	WARNING,
	ERROR
};

/**
 * A library of log utility functions
 */
UCLASS()
class DEBUG_API ULog final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Log whether the object is valid or not
	static void LogObjectValidity(UObject* ObjectRef, bool bLogInViewport = false);

	// Log a debug message to the console or viewport
	static void LogDebugMessage(ELogType LogLevel, const FString& LogMessage, bool bLogInViewport = false, float TimeToDisplay = 5.0f);	
	
};
