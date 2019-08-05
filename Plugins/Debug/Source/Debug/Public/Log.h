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
	static void ObjectValidity(UObject* ObjectRef, bool bLogInViewport = false);

	// Log a debug message to the console or viewport
	static void DebugMessage(ELogType LogLevel, const FString& LogMessage, bool bLogInViewport = false, float TimeToDisplay = 5.0f);	

	// Log a hello message to the console or viewport
	static void LogHello(bool bLogInViewport = false);

	// Log a yes message to the console or viewport
	static void LogYes(bool bLogInViewport = false);

	// Log a no message to the console or viewport
	static void LogNo(bool bLogInViewport = false);

};
