// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OverthroneFunctionLibrary.generated.h"

/**
 * A list of useful utility and helpful public functions exposed to blueprints
 */
UCLASS()
class MYTHOLOGICALBOSS_API UOverthroneFunctionLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Get the project version string from DefaultGame.ini file
	UFUNCTION(BlueprintPure, Category = "Project Information")
		static FString GetProjectVersion();
	
	// Get the first boss character found in the level
	UFUNCTION(BlueprintPure, Category = "Overthrone")
		static class AOverthroneCharacter* GetBossCharacter(UWorld* InWorld);

	// Get our custom game instance
	UFUNCTION(BlueprintPure, Category = "Overthrone")
		static class UOverthroneGameInstance* GetGameInstance(const UObject* WorldContext);

	// Get our custom HUD object
	UFUNCTION(BlueprintPure, Category = "Overthrone")
		static class AOverthroneHUD* GetHUD(APlayerController* InPlayerController);

	// Get our master HUD widget
	UFUNCTION(BlueprintPure, Category = "Overthrone")
		static class UMasterHUD* GetMasterHUD(APlayerController* InPlayerController);

	// Get our custom game mode
	UFUNCTION(BlueprintPure, Category = "Overthrone")
		static class ABossBattleGameMode* GetGameMode(const UObject* WorldContext);
};
