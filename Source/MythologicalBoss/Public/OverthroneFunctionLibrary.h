// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OverthroneEnums.h"
#include "Components/TimelineComponent.h"
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

	// Get the current project build configuration type as a string
	UFUNCTION(BlueprintPure, Category = "Project Information")
		static FString GetBuildConfigurationAsString();

	// Get the current project build configuration type
	UFUNCTION(BlueprintPure, Category = "Project Information")
		static TEnumAsByte<EBuildConfig> GetBuildConfiguration();
	
	// Get the first player character found in the level
	UFUNCTION(BlueprintPure, Category = "Overthrone")
		static class AOverthroneCharacter* GetPlayerCharacter(UObject* WorldContextObject);

	// Get the first boss character found in the level
	UFUNCTION(BlueprintPure, Category = "Overthrone")
		static class AOverthroneCharacter* GetBossCharacter(UObject* Object, const FName& Tag);

	// Get our custom game instance
	UFUNCTION(BlueprintPure, Category = "Overthrone")
		static class UOverthroneGameInstance* GetGameInstance(const UObject* WorldContext);

	// Get our custom game state
	UFUNCTION(BlueprintPure, Category = "Overthrone")
		static class AOverthroneGameState* GetGameState(const UObject* WorldContext);

	// Get our custom HUD object
	UFUNCTION(BlueprintPure, Category = "Overthrone")
		static class AOverthroneHUD* GetHUD(APlayerController* InPlayerController);

	// Get our master HUD widget
	UFUNCTION(BlueprintPure, Category = "Overthrone")
		static class UMasterHUD* GetMasterHUD(APlayerController* InPlayerController);

	// Get our custom game mode
	UFUNCTION(BlueprintPure, Category = "Overthrone")
		static class ABossBattleGameMode* GetGameMode(const UObject* WorldContext);

	// Pauses the game
	UFUNCTION(BlueprintCallable, Category = "Overthrone")
		static void PauseGame(const UObject* WorldContext, APlayerController* InPlayerController);

	// UnPauses the game
	UFUNCTION(BlueprintCallable, Category = "Overthrone")
		static void UnPauseGame(const UObject* WorldContext, APlayerController* InPlayerController);

	// Check if gamepad is connected at runtime
	UFUNCTION(BlueprintPure, Category = "System Information")
		static bool IsGamepadConnected();

	UFUNCTION(BlueprintPure, Category = "Input")
		static FString ConvertKeyToString(FKey Key);

	static FVector SmoothStop(const FVector& A, const FVector& B, float Alpha, float Exp);

	static void SetupTimeline(UObject* Object, FTimeline& InTimeline, class UCurveFloat* InCurveFloat, const bool bLooping, float InPlaybackSpeed, const FName& TimelineCallbackFuncName, const FName& TimelineFinishedCallbackFuncName = NAME_None);
};
