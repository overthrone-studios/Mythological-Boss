// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Widgets/HUD/HUDBase.h"
#include "TutorialHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCompletedSignature);

/**
 * Main HUD for the tutorial section of the game
 */
UCLASS()
class MYTHOLOGICALBOSS_API UTutorialHUD final : public UHUDBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Tutorial HUD")
		void SlideIn();	
	
	UFUNCTION(BlueprintCallable, Category = "Tutorial HUD")
		void SlideOut();

	void ProgressTutorial();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Tutorial HUD")
		void OnTutorialCompleted();

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Tutorial HUD | Animations")
		UWidgetAnimation* SlideAnim;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Tutorial HUD")
		TArray<class UTutorialData*> Tutorials;
};
