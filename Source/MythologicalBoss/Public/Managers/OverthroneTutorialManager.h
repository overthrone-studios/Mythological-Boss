// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OverthroneTutorialManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginTutorialSignature);

UCLASS(BlueprintType)
class MYTHOLOGICALBOSS_API AOverthroneTutorialManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AOverthroneTutorialManager();

	UPROPERTY(BlueprintAssignable)
		FOnBeginTutorialSignature OnBeginTutorial;

	UFUNCTION(BlueprintCallable, Category = "Tutorial Manager")
		void BeginTutorial();
	
	UFUNCTION(BlueprintCallable, Category = "Tutorial Manager")
		void EndTutorial();

	UFUNCTION(BlueprintCallable, Category = "Tutorial Manager")
		void NextTutorial();

protected:
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial Manager")
		TSubclassOf<class UTutorialHUD> TutorialHUDClass;

	UPROPERTY(BlueprintReadOnly, Category = "Tutorial Manager")
		class UTutorialHUD* TutorialHUD;

	APlayerController* PlayerController;
};
