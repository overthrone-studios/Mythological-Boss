// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OverthroneFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MYTHOLOGICALBOSS_API UOverthroneFunctionLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Get the project version string from DefaultGame.ini file
	UFUNCTION(BlueprintPure, Category = "Project Information")
		static FString GetProjectVersion();
	
};
