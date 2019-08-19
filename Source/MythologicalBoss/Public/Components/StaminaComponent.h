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

	// Return the actor's default stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetDefaultStamina() const { return DefaultStamina; }

	// Return the actor's current stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetCurrentStamina() const { return Stamina; }

	// Return the actor's previous stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetPreviousStamina() const { return PreviousStamina; }

	// Return the actor's new stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetSmoothedStamina() const { return NewStamina; }

	// Returns true if smooth bar is enabled
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE bool IsUsingSmoothBar() const { return bSmoothBar; }

	// Set a new current stamina value
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void SetStamina(float NewStaminaAmount);

	// Set the new stamina value
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void SetSmoothedStamina(float Value);

	// Add stamina by an amount
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void IncreaseStamina(float Amount);

	// Subtract stamina by an amount
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void DecreaseStamina(float Amount);

	// Resets stamina back to default
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void ResetStamina();


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

	// Should we use a smooth stamina bar?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina")
		uint8 bSmoothBar : 1;

	// The rate of stamina regeneration (How fast do we replenish stamina)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f))
		float RegenerationRate = 200.0f;

	// The amount of time (in seconds) we should wait before regenerating stamina
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f, ClampMax = 60.0f))
		float RegenerationDelay = 1.0f;

	// The stamina value to subtract when light attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f))
		float LightAttack = 10.0f;

	// The stamina value to subtract when heavy attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f))
		float HeavyAttack = 30.0f;

	// The stamina value to subtract when dashing
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f))
		float Dash = 40.0f;

	// The stamina value to subtract while running
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f))
		float Run = 100.0f;

	// The stamina value to subtract when being hit while blocking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f))
		float ShieldHit = 300.0f;

	// The actor's previous health before being damaged
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
		float PreviousStamina;

	// The actor's updated health when bSmoothBar is enabled
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
		float NewStamina;
};
