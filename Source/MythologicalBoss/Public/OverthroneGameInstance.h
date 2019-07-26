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

	void InitInstance();

	UPROPERTY(BlueprintReadWrite, Category = "Overthrone Game Instance | Boss Info")
		float PlayerStartingHealth{};

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game Instance | Boss Info")
		float PlayerHealth{};

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game Instance | Boss Info")
		float PlayerStartingStamina{};

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game Instance | Boss Info")
		float PlayerStamina{};

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game Instance | Boss Info")
		FVector BossLocation{};

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game Instance | Boss Info")
		float BossHealth{};

	UPROPERTY(BlueprintReadOnly, Category = "Overthrone Game Instance | Boss Info")
		float BossStartingHealth{};

private:
	class UUserWidget* PauseMenu{};
	TSubclassOf<class UUserWidget> PauseMenuWidgetClass;

	APlayerController* PlayerController{};
};
