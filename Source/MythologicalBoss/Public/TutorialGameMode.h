// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TutorialGameMode.generated.h"

/**
 * Used in the tutorial map
 */
UCLASS()
class MYTHOLOGICALBOSS_API ATutorialGameMode final : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATutorialGameMode();

	UFUNCTION(BlueprintPure, Category = "Tutorial Game Mode")
		class UTutorialHUD* GetTutorialHUD() const { return TutorialHUD; }

	UPROPERTY(BlueprintReadOnly, Category = "Tutorial Game Mode")
		uint8 bTutorialStarted : 1;

protected:
	void BeginPlay() override;

	void StartTutorial();

private:
	TSubclassOf<UUserWidget> TutorialHUDClass;
	class UTutorialHUD* TutorialHUD;

	void InitHUD();

	FTimerHandle TutorialStartTimer;
};
