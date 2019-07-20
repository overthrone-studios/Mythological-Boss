// Copyright Overthrone Studios 2019

#pragma once

#include "Engine/GameInstance.h"
#include "OverthroneGameInstance.generated.h"

/**
 * High-level manager object for Overthrone game
 */
UCLASS(config=Game)
class MYTHOLOGICALBOSS_API UOverthroneGameInstance final : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UOverthroneGameInstance();

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		bool IsGamePaused();

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		void PauseGame();

	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		void UnPauseGame();

	// Switches input mode to UI only
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		void SetInputModeUI() const;

	// Switches input mode to Game only
	UFUNCTION(BlueprintCallable, Category = "Overthrone Game Instance")
		void SetInputModeGame() const;

protected:
	void OnStart() override;

private:
	class UMenuBase* PauseMenu{};
	TSubclassOf<class UMenuBase> PauseMenuWidgetClass;

	APlayerController* PlayerController{};
};
