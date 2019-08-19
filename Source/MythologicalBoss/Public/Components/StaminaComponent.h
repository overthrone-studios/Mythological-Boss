// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Tags", "Activation", "Cooking", "AssetUserData", "Collision"))
class MYTHOLOGICALBOSS_API UStaminaComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStaminaComponent();

protected:
	void BeginPlay() override;

	// Initialize the actor's stamina values
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void InitStamina();

	#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

	// The actor's current stamina
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = 0.0f))
		float Stamina = 100.0f;

	// The actor's default health
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = 0.0f))
		float DefaultStamina = 100.0f;

	// Should we use a smooth health bar?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina")
		uint8 bSmoothBar : 1;

	// The actor's previous health before being damaged
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
		float PreviousStamina;

	// The actor's updated health when bSmoothBar is enabled
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
		float NewStamina;
};
